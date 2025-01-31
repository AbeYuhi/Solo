#include "PostEffectManager.h"

/// <summary>
/// PostEffectManager.h
/// ポストエフェクトを管理しているクラスの実装ファイル
/// </summary>

namespace MyEngine {

	PostEffectManager* PostEffectManager::GetInstance() {
		static PostEffectManager instance;
		return &instance;
	}

	void PostEffectManager::Initialize() {
		DirectXCommon* directX = DirectXCommon::GetInstance();

		postEffect_ = kNone;

		//Rendertextureの生成
		CreateRenderTexture();

		//ルートシグネチャーの生成
		CreateRootSignature();

		//PSOの生成
		CreatePSO();

		//Respourceの確保
		//HSVMaterial
		hsvMaterialResource_ = CreateBufferResource(sizeof(HSVMaterial));
		hsvMaterialResource_->Map(0, nullptr, reinterpret_cast<void**>(&hsvMaterial_));
		hsvMaterial_->hue = 0.0f;
		hsvMaterial_->saturation = 0.0f;
		hsvMaterial_->value = 0.0f;
		//smoothing
		smoothingInfoResource_ = CreateBufferResource(sizeof(SmoothingInfo));
		smoothingInfoResource_->Map(0, nullptr, reinterpret_cast<void**>(&smoothingInfo_));
		smoothingInfo_->kernelSize = 1;
		smoothingInfo_->type = 0;
		smoothingInfo_->blurStrength = 2.0f;

		//radialBlur
		radialBlurInfoResource_ = CreateBufferResource(sizeof(RadialBlurInfo));
		radialBlurInfoResource_->Map(0, nullptr, reinterpret_cast<void**>(&radialBlurInfo_));
		radialBlurInfo_->numSamples = 20;
		radialBlurInfo_->blurWidth = 0.005f;

		//vignetteBlur
		vignetteBlurInfoResource_ = CreateBufferResource(sizeof(VignetteBlurInfo));
		vignetteBlurInfoResource_->Map(0, nullptr, reinterpret_cast<void**>(&vignetteBlurInfo_));
		vignetteBlurInfo_->intensity = 0.5f;
		vignetteBlurInfo_->blurAmount = 0.5f;

		//Fog
		//リソースの確保
		depthTextureResource_ = directX->GetDsvResource();
		//Srvの作成
		depthSrvIndex_ = SrvManager::GetInstance()->Allocate();
		D3D12_SHADER_RESOURCE_VIEW_DESC depthTextureSrvDesc{};
		depthTextureSrvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		depthTextureSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		depthTextureSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		depthTextureSrvDesc.Texture2D.MipLevels = 1;
		D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = SrvManager::GetInstance()->GetCPUDescriptorHandle(depthSrvIndex_);
		directX->GetDevice()->CreateShaderResourceView(depthTextureResource_, &depthTextureSrvDesc, handleCPU);
		//Viewを送るためのリソースの生成
		//Resourceの生成
		inverseProjectionResorce_ = MyEngine::CreateBufferResource(sizeof(Matrix4x4));
		//Resourceにデータを書き込む
		inverseProjectionResorce_->Map(0, nullptr, reinterpret_cast<void**>(&inverseProjectionData_));
		*inverseProjectionData_ = MakeIdentity4x4();
	}

	void PostEffectManager::PreDraw() {
		DirectXCommon* directX = DirectXCommon::GetInstance();

		//ポストエフェクトをしないときは通常描画するように
		if (postEffect_ == PostEffect::kNone) {
			NormalPreDraw();
			directX->ClearDepthStencilBuffer();
		}
		else {
			//ポストエフェクト時の処理
			RenderPreDraw();
		}
	}

	void PostEffectManager::PostDraw() {
		//通常物の描画
		DrawManager::GetInstance()->Draw();

		//ポストエフェクト時に通過するように
		if (postEffect_ != PostEffect::kNone) {
			RenderPostDraw();
		}

		//UIの描画
		DrawManager::GetInstance()->ForegroundSpritesDraw();
	}

	void PostEffectManager::NormalPreDraw() {
		DirectXCommon* directX = DirectXCommon::GetInstance();

		//バックバッファの取得
		UINT backBufferIndex = directX->GetSwapShain()->GetCurrentBackBufferIndex();

		//バックバッファのハンドルの取得
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = directX->GetRtvHandle(backBufferIndex);

		//TransitionBarrierの設定
		D3D12_RESOURCE_BARRIER barrier{};
		//今回のバリアの種類
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = directX->GetBackBuffer(backBufferIndex);
		//遷移前のResourceState
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		//遷移後のResoruceState
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		//TransitionBarrierを張る
		directX->GetCommandList()->ResourceBarrier(1, &barrier);

		//DSVのBarrierの設定
		D3D12_RESOURCE_BARRIER dsvBarrier{};
		//今回のバリアの種類
		dsvBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		dsvBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		dsvBarrier.Transition.pResource = depthTextureResource_;
		//遷移前のResourceState
		dsvBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		//遷移後のResoruceState
		dsvBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;
		//TransitionBarrierを張る
		directX->GetCommandList()->ResourceBarrier(1, &dsvBarrier);

		//RTVの設定
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = directX->GetDsvHandle();
		directX->GetCommandList()->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

		//画面のクリア
		directX->ClearRenderTarget({ 0.1f, 0.25f, 0.5f, 1.0f }, rtvHandle);

		//描画用DescriptorHeapの設定
		ID3D12DescriptorHeap* descriptorHeaps[] = { SrvManager::GetInstance()->GetSrvDescriptorHeap() };
		directX->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
	}

	void PostEffectManager::RenderPreDraw() {
		DirectXCommon* directX = DirectXCommon::GetInstance();

		//バックバッファのハンドルの取得
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = directX->GetRtvHandle(kRTVIndex);

		//TransitionBarrierの設定
		D3D12_RESOURCE_BARRIER barrier{};
		//今回のバリアの種類
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = renderTextureResource_.Get();
		//遷移前のResourceState
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		//遷移後のResoruceState
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		//TransitionBarrierを張る
		directX->GetCommandList()->ResourceBarrier(1, &barrier);

		//DSVのBarrierの設定
		D3D12_RESOURCE_BARRIER dsvBarrier{};
		//今回のバリアの種類
		dsvBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		dsvBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		dsvBarrier.Transition.pResource = depthTextureResource_;
		//遷移前のResourceState
		dsvBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		//遷移後のResoruceState
		dsvBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;
		//TransitionBarrierを張る
		directX->GetCommandList()->ResourceBarrier(1, &dsvBarrier);

		//RTVの設定
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = directX->GetDsvHandle();
		directX->GetCommandList()->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

		//画面のクリア
		//指定した色で画面のクリア
		directX->ClearRenderTarget({ 1.0f, 0.0f, 0.0f, 1.0f }, rtvHandle);
		//深度バッファのクリア
		directX->ClearDepthStencilBuffer();

		//描画用DescriptorHeapの設定
		ID3D12DescriptorHeap* descriptorHeaps[] = { SrvManager::GetInstance()->GetSrvDescriptorHeap() };
		directX->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
	}

	void PostEffectManager::RenderPostDraw() {
		DirectXCommon* directX = DirectXCommon::GetInstance();

		//TransitionBarrierの設定
		D3D12_RESOURCE_BARRIER renderBarrier{};
		//今回のバリアの種類
		renderBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		renderBarrier.Transition.pResource = renderTextureResource_.Get();
		//遷移前のResourceState
		renderBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		//遷移後のResoruceState
		renderBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		//TransitionBarrierを張る
		directX->GetCommandList()->ResourceBarrier(1, &renderBarrier);

		//スワップチェインに表示するためセット
		NormalPreDraw();

		//DSVのBarrierの設定
		D3D12_RESOURCE_BARRIER dsvBarrier{};
		//今回のバリアの種類
		dsvBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		dsvBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		dsvBarrier.Transition.pResource = depthTextureResource_;
		//遷移前のResourceState
		dsvBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_DEPTH_WRITE;
		//遷移後のResoruceState
		dsvBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		//TransitionBarrierを張る
		directX->GetCommandList()->ResourceBarrier(1, &dsvBarrier);

		//描画処理の記入
		GraphicsPipelineManager* psoManager = GraphicsPipelineManager::GetInstance();
		//ViewPortの設定
		directX->GetCommandList()->RSSetViewports(1, psoManager->GetViewPort());
		//Scirssorの設定
		directX->GetCommandList()->RSSetScissorRects(1, psoManager->GetScissorRect());
		directX->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		directX->GetCommandList()->SetPipelineState(graphicsPipelineState_[postEffect_]->Get());
		directX->GetCommandList()->SetGraphicsRootSignature(rootSignature_[postEffect_].Get());
		directX->GetCommandList()->SetGraphicsRootDescriptorTable(0, SrvManager::GetInstance()->GetGPUDescriptorHandle(srvIndex_));

		//カメラの情報を取得
		*inverseProjectionData_ = MainCamera::GetInstance()->GetProjectionMatrix();
		*inverseProjectionData_ = Inverse(*inverseProjectionData_);
		
		//CBufferで送る場合
		switch (postEffect_)
		{
		case kCopy:

			break;
		case kHSVFilter:
			directX->GetCommandList()->SetGraphicsRootConstantBufferView(1, hsvMaterialResource_->GetGPUVirtualAddress());
			break;
		case kGrayScale:

			break;
		case kSepiaScale:

			break;
		case kVignette:

			break;
		case kVignetteBlur:
			directX->GetCommandList()->SetGraphicsRootConstantBufferView(1, vignetteBlurInfoResource_->GetGPUVirtualAddress());
			break;
		case kSmoothing:
			directX->GetCommandList()->SetGraphicsRootConstantBufferView(1, smoothingInfoResource_->GetGPUVirtualAddress());
			break;
		case kRadialBlur:
			directX->GetCommandList()->SetGraphicsRootConstantBufferView(1, radialBlurInfoResource_->GetGPUVirtualAddress());
			break;
		case kFog:
			directX->GetCommandList()->SetGraphicsRootDescriptorTable(1, SrvManager::GetInstance()->GetGPUDescriptorHandle(depthSrvIndex_));
			directX->GetCommandList()->SetGraphicsRootConstantBufferView(2, inverseProjectionResorce_->GetGPUVirtualAddress());
			break;
		default:
			break;
		}
		directX->GetCommandList()->DrawInstanced(3, 1, 0, 0);
	}

	void PostEffectManager::CreateRenderTexture() {
		DirectXCommon* directXCommon = DirectXCommon::GetInstance();

		//RTVの生成
		const Vector4 kRenderTargetClearValue{ 1.0f, 0.0f, 0.0f, 1.0f }; //一旦赤色
		renderTextureResource_ = CreateRenderTextureResoruce(WinApp::kWindowWidth, WinApp::kWindowHeight, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, kRenderTargetClearValue);
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = directXCommon->GetRtvHandle(kRTVIndex);
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = directXCommon->GetRtvDesc();
		directXCommon->GetDevice()->CreateRenderTargetView(renderTextureResource_.Get(), &rtvDesc, rtvHandle);

		//SRVの生成
		srvIndex_ = SrvManager::GetInstance()->Allocate();
		D3D12_SHADER_RESOURCE_VIEW_DESC renderTextureSrvDesc{};
		renderTextureSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		renderTextureSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		renderTextureSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		renderTextureSrvDesc.Texture2D.MipLevels = 1;
		D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = SrvManager::GetInstance()->GetCPUDescriptorHandle(srvIndex_);
		directXCommon->GetDevice()->CreateShaderResourceView(renderTextureResource_.Get(), &renderTextureSrvDesc, handleCPU);
	}

	void PostEffectManager::CreateRootSignature() {
		//DirectXCommonの取得
		DirectXCommon* directXCommon = DirectXCommon::GetInstance();

		ComPtr<ID3DBlob> signatureBlob[PostEffect::kCountOfPostEffect];
		ComPtr<ID3DBlob> errorBlob[PostEffect::kCountOfPostEffect];

		for (int shaderPack = 0; shaderPack < PostEffect::kCountOfPostEffect; shaderPack++) {
			switch (shaderPack)
			{
			case PostEffect::kVignetteBlur:
			case PostEffect::kHSVFilter:
			case PostEffect::kSmoothing:
			case PostEffect::kRadialBlur:
			{
				//DescriptorRangeの設定
				D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
				descriptorRange[0].BaseShaderRegister = 0;
				descriptorRange[0].NumDescriptors = 1;
				descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
				descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

				//RootSignature生成
				D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
				descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

				//RootParameter作成。
				D3D12_ROOT_PARAMETER rootParameters[2] = {};
				rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
				rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
				rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;
				rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);
				rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
				rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
				rootParameters[1].Descriptor.ShaderRegister = 0;
				descriptionRootSignature.pParameters = rootParameters;
				descriptionRootSignature.NumParameters = _countof(rootParameters);

				//Sampler
				D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
				staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
				staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
				staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
				staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
				staticSamplers[0].BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
				staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
				staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
				staticSamplers[0].ShaderRegister = 0;
				staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
				descriptionRootSignature.pStaticSamplers = staticSamplers;
				descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

				//シリアライズしてバイナリする
				LRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob[shaderPack], &errorBlob[shaderPack]);
				if (FAILED(hr)) {
					Log(reinterpret_cast<char*>(errorBlob[shaderPack]->GetBufferPointer()));
					assert(false);
				}
				//バイナリをもとに生成
				hr = directXCommon->GetDevice()->CreateRootSignature(0, signatureBlob[shaderPack]->GetBufferPointer(), signatureBlob[shaderPack]->GetBufferSize(), IID_PPV_ARGS(&rootSignature_[shaderPack]));
				assert(SUCCEEDED(hr));
				break;
			}
			case kFog:
			{
					//DescriptorRangeの設定
					D3D12_DESCRIPTOR_RANGE descriptorRange[2] = {};
					descriptorRange[0].BaseShaderRegister = 0;
					descriptorRange[0].NumDescriptors = 1;
					descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
					descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
					descriptorRange[1].BaseShaderRegister = 1;
					descriptorRange[1].NumDescriptors = 1;
					descriptorRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
					descriptorRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

					//RootSignature生成
					D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
					descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

					//RootParameter作成。
					D3D12_ROOT_PARAMETER rootParameters[3] = {};
					rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
					rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
					rootParameters[0].DescriptorTable.pDescriptorRanges = &descriptorRange[0];
					rootParameters[0].DescriptorTable.NumDescriptorRanges = descriptorRange[0].NumDescriptors;
					rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
					rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
					rootParameters[1].DescriptorTable.pDescriptorRanges = &descriptorRange[1];
					rootParameters[1].DescriptorTable.NumDescriptorRanges = descriptorRange[1].NumDescriptors;
					rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
					rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
					rootParameters[2].Descriptor.ShaderRegister = 0;
					descriptionRootSignature.pParameters = rootParameters;
					descriptionRootSignature.NumParameters = _countof(rootParameters);

					//Sampler
					D3D12_STATIC_SAMPLER_DESC staticSamplers[2] = {};
					staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
					staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
					staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
					staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
					staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
					staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
					staticSamplers[0].ShaderRegister = 0;
					staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
					staticSamplers[1].Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
					staticSamplers[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
					staticSamplers[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
					staticSamplers[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
					staticSamplers[1].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
					staticSamplers[1].MaxLOD = D3D12_FLOAT32_MAX;
					staticSamplers[1].ShaderRegister = 1;
					staticSamplers[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
					descriptionRootSignature.pStaticSamplers = staticSamplers;
					descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

				//シリアライズしてバイナリする
				LRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob[shaderPack], &errorBlob[shaderPack]);
				if (FAILED(hr)) {
					Log(reinterpret_cast<char*>(errorBlob[shaderPack]->GetBufferPointer()));
					assert(false);
				}
				//バイナリをもとに生成
				hr = directXCommon->GetDevice()->CreateRootSignature(0, signatureBlob[shaderPack]->GetBufferPointer(), signatureBlob[shaderPack]->GetBufferSize(), IID_PPV_ARGS(&rootSignature_[shaderPack]));
				assert(SUCCEEDED(hr));
				break;
			}
			default:
			{
				//DescriptorRangeの設定
				D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
				descriptorRange[0].BaseShaderRegister = 0;
				descriptorRange[0].NumDescriptors = 1;
				descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
				descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

				//RootSignature生成
				D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
				descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

				//RootParameter作成。
				D3D12_ROOT_PARAMETER rootParameters[1] = {};
				rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
				rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
				rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;
				rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);
				descriptionRootSignature.pParameters = rootParameters;
				descriptionRootSignature.NumParameters = _countof(rootParameters);

				//Sampler
				D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
				staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
				staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
				staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
				staticSamplers[0].ShaderRegister = 0;
				staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
				descriptionRootSignature.pStaticSamplers = staticSamplers;
				descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

				//シリアライズしてバイナリする
				LRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob[shaderPack], &errorBlob[shaderPack]);
				if (FAILED(hr)) {
					Log(reinterpret_cast<char*>(errorBlob[shaderPack]->GetBufferPointer()));
					assert(false);
				}
				//バイナリをもとに生成
				hr = directXCommon->GetDevice()->CreateRootSignature(0, signatureBlob[shaderPack]->GetBufferPointer(), signatureBlob[shaderPack]->GetBufferSize(), IID_PPV_ARGS(&rootSignature_[shaderPack]));
				assert(SUCCEEDED(hr));
				break;
			}
			}
		}
	}

	void PostEffectManager::CreatePSO() {
		DirectXCommon* directXCommon = DirectXCommon::GetInstance();

		//InputLayoutの設定
		D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};
		inputLayoutDesc.pInputElementDescs = nullptr;
		inputLayoutDesc.NumElements = 0;

		//BlendStateの設定
		D3D12_BLEND_DESC blendDesc[BlendMode::kCountOfBlendMode] = { 0 };
		for (int blendMode = 0; blendMode < BlendMode::kCountOfBlendMode; blendMode++) {
			switch (blendMode)
			{
			case BlendMode::kBlendModeNone:
				blendDesc[blendMode].RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				break;
			case BlendMode::kBlendModeNormal:
				blendDesc[blendMode].RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				blendDesc[blendMode].RenderTarget[0].BlendEnable = TRUE;
				blendDesc[blendMode].RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
				blendDesc[blendMode].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
				blendDesc[blendMode].RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
				blendDesc[blendMode].RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
				blendDesc[blendMode].RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
				blendDesc[blendMode].RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
				break;
			case BlendMode::kBlendModeAdd:
				blendDesc[blendMode].RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				blendDesc[blendMode].RenderTarget[0].BlendEnable = TRUE;
				blendDesc[blendMode].RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
				blendDesc[blendMode].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
				blendDesc[blendMode].RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
				blendDesc[blendMode].RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
				blendDesc[blendMode].RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
				blendDesc[blendMode].RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
				break;
			case BlendMode::kBlendModeSubtract:
				blendDesc[blendMode].RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				blendDesc[blendMode].RenderTarget[0].BlendEnable = TRUE;
				blendDesc[blendMode].RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
				blendDesc[blendMode].RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
				blendDesc[blendMode].RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
				blendDesc[blendMode].RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
				blendDesc[blendMode].RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
				blendDesc[blendMode].RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
				break;
			case BlendMode::kBlendModeMultily:
				blendDesc[blendMode].RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				blendDesc[blendMode].RenderTarget[0].BlendEnable = TRUE;
				blendDesc[blendMode].RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
				blendDesc[blendMode].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
				blendDesc[blendMode].RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
				blendDesc[blendMode].RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
				blendDesc[blendMode].RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
				blendDesc[blendMode].RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
				break;
			case BlendMode::kBlendModeScreen:
				blendDesc[blendMode].RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				blendDesc[blendMode].RenderTarget[0].BlendEnable = TRUE;
				blendDesc[blendMode].RenderTarget[0].SrcBlend = D3D12_BLEND_INV_SRC_COLOR;
				blendDesc[blendMode].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
				blendDesc[blendMode].RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
				blendDesc[blendMode].RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
				blendDesc[blendMode].RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
				blendDesc[blendMode].RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
				break;
			default:
				break;
			}
		}

		//RasiterzerStatesの設定
		D3D12_RASTERIZER_DESC rasterizerDesc{};
		//裏側を表示しない
		rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
		//三角形の中を塗りつぶす
		rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

		//Shaderのコンパイル
		ComPtr<IDxcBlob> vertexShaderBlob[PostEffect::kCountOfPostEffect]{ 0 };
		ComPtr<IDxcBlob> pixelShaderBlob[PostEffect::kCountOfPostEffect]{ 0 };
		for (int shaderPack = 0; shaderPack < PostEffect::kCountOfPostEffect; shaderPack++) {
			switch (shaderPack)
			{
			case PostEffect::kNone:
			default:
			case PostEffect::kCopy:
				//頂点シェーダー
				vertexShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/FullScreen.VS.hlsl", L"vs_6_0");
				assert(vertexShaderBlob[shaderPack] != nullptr);
				//ピクセルシェーダー
				pixelShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/CopyImage.PS.hlsl", L"ps_6_0");
				assert(pixelShaderBlob[shaderPack] != nullptr);
				break;
			case PostEffect::kHSVFilter:
				//頂点シェーダー
				vertexShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/FullScreen.VS.hlsl", L"vs_6_0");
				assert(vertexShaderBlob[shaderPack] != nullptr);
				//ピクセルシェーダー
				pixelShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/HSVFilter.PS.hlsl", L"ps_6_0");
				assert(pixelShaderBlob[shaderPack] != nullptr);
				break;
			case PostEffect::kGrayScale:
				//頂点シェーダー
				vertexShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/FullScreen.VS.hlsl", L"vs_6_0");
				assert(vertexShaderBlob[shaderPack] != nullptr);
				//ピクセルシェーダー
				pixelShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/GrayScale.PS.hlsl", L"ps_6_0");
				assert(pixelShaderBlob[shaderPack] != nullptr);
				break;
			case PostEffect::kSepiaScale:
				//頂点シェーダー
				vertexShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/FullScreen.VS.hlsl", L"vs_6_0");
				assert(vertexShaderBlob[shaderPack] != nullptr);
				//ピクセルシェーダー
				pixelShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/SepiaScale.PS.hlsl", L"ps_6_0");
				assert(pixelShaderBlob[shaderPack] != nullptr);
				break;
			case PostEffect::kVignette:
				//頂点シェーダー
				vertexShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/FullScreen.VS.hlsl", L"vs_6_0");
				assert(vertexShaderBlob[shaderPack] != nullptr);
				//ピクセルシェーダー
				pixelShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/Vignette.PS.hlsl", L"ps_6_0");
				assert(pixelShaderBlob[shaderPack] != nullptr);
				break;
			case PostEffect::kVignetteBlur:
				//頂点シェーダー
				vertexShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/FullScreen.VS.hlsl", L"vs_6_0");
				assert(vertexShaderBlob[shaderPack] != nullptr);
				//ピクセルシェーダー
				pixelShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/VignetteBlur.PS.hlsl", L"ps_6_0");
				assert(pixelShaderBlob[shaderPack] != nullptr);
				break;
			case PostEffect::kSmoothing:
				//頂点シェーダー
				vertexShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/FullScreen.VS.hlsl", L"vs_6_0");
				assert(vertexShaderBlob[shaderPack] != nullptr);
				//ピクセルシェーダー
				pixelShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/Smoothing.PS.hlsl", L"ps_6_0");
				assert(pixelShaderBlob[shaderPack] != nullptr);
				break;
			case PostEffect::kRadialBlur:
				//頂点シェーダー
				vertexShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/FullScreen.VS.hlsl", L"vs_6_0");
				assert(vertexShaderBlob[shaderPack] != nullptr);
				//ピクセルシェーダー
				pixelShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/RadialBlur.PS.hlsl", L"ps_6_0");
				assert(pixelShaderBlob[shaderPack] != nullptr);
				break;
			case PostEffect::kFog:
				//頂点シェーダー
				vertexShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/FullScreen.VS.hlsl", L"vs_6_0");
				assert(vertexShaderBlob[shaderPack] != nullptr);
				//ピクセルシェーダー
				pixelShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/Fog.PS.hlsl", L"ps_6_0");
				assert(pixelShaderBlob[shaderPack] != nullptr);
				break;
			}
		}

		//DepthStencilStateの設定
		D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
		depthStencilDesc.DepthEnable = false;

		//PSOの生成
		D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipeLineStateDesc{};
		for (int shaderPack = 0; shaderPack < PostEffect::kCountOfPostEffect; shaderPack++) {
			for (int blendMode = 0; blendMode < BlendMode::kCountOfBlendMode; blendMode++) {
				graphicsPipeLineStateDesc.pRootSignature = rootSignature_[shaderPack].Get();
				graphicsPipeLineStateDesc.InputLayout = inputLayoutDesc;
				graphicsPipeLineStateDesc.VS = { vertexShaderBlob[shaderPack]->GetBufferPointer(), vertexShaderBlob[shaderPack]->GetBufferSize() };
				graphicsPipeLineStateDesc.PS = { pixelShaderBlob[shaderPack]->GetBufferPointer(), pixelShaderBlob[shaderPack]->GetBufferSize() };
				graphicsPipeLineStateDesc.BlendState = blendDesc[blendMode];
				graphicsPipeLineStateDesc.RasterizerState = rasterizerDesc;
				graphicsPipeLineStateDesc.DepthStencilState = depthStencilDesc;
				graphicsPipeLineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
				//書き込むRTVの情報
				graphicsPipeLineStateDesc.NumRenderTargets = 1;
				graphicsPipeLineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
				//利用する形状タイプ
				if (shaderPack == PipelineState::kWireFrame) {
					graphicsPipeLineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
				}
				else {
					graphicsPipeLineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
				}

				//どのように画面に色を打ち込むのかの設定
				graphicsPipeLineStateDesc.SampleDesc.Count = 1;
				graphicsPipeLineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
				//実際に生成
				[[maybe_unused]] LRESULT hr = directXCommon->GetDevice()->CreateGraphicsPipelineState(&graphicsPipeLineStateDesc, IID_PPV_ARGS(&graphicsPipelineState_[shaderPack][blendMode]));
				assert(SUCCEEDED(hr));
			}
		}
	}

	ComPtr<ID3D12Resource> PostEffectManager::CreateRenderTextureResoruce(uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor) {
		//頂点リソース用のヒープの設定
		D3D12_HEAP_PROPERTIES heapProperties{};
		heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
		//頂点リソースの設定
		D3D12_RESOURCE_DESC resourceDesc{};
		//バックバッファリソース
		resourceDesc.Width = width;
		resourceDesc.Height = height;
		resourceDesc.MipLevels = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.Format = format;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		//クリア値
		D3D12_CLEAR_VALUE clearValue;
		clearValue.Format = format;
		clearValue.Color[0] = clearColor.x;
		clearValue.Color[1] = clearColor.y;
		clearValue.Color[2] = clearColor.z;
		clearValue.Color[3] = clearColor.w;
		//実際に生成
		ComPtr<ID3D12Resource> resource;
		[[maybe_unused]] LRESULT hr = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			&clearValue,
			IID_PPV_ARGS(&resource));
		assert(SUCCEEDED(hr));
		return resource;
	}

}