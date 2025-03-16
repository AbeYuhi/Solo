#include "ComputePipelineManager.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"

/// <summary>
/// GraphicPipelineManager.h
/// GraphicPipelineを管理しているクラスの実装ファイル
/// </summary>

namespace MyEngine {

	ComputePipelineManager* ComputePipelineManager::GetInstance() {
		static ComputePipelineManager instance;
		return &instance;
	}

	void ComputePipelineManager::Initialize() {
		CreateComputePipelineManager();
	}

	void ComputePipelineManager::CreateComputePipelineManager() {
		//ルートシグネチャーの生成
		CreateRootSignature();

		//PSOの生成
		CreatePSO();
	}

	void ComputePipelineManager::CreateRootSignature() {
		//DirectXCommonの取得
		DirectXCommon* directXCommon = DirectXCommon::GetInstance();

		ComPtr<ID3DBlob> signatureBlob[ComputePipelineState::kCountOfComputePipelineState];
		ComPtr<ID3DBlob> errorBlob[ComputePipelineState::kCountOfComputePipelineState];
		for (int shaderPack = 0; shaderPack < ComputePipelineState::kCountOfComputePipelineState; shaderPack++) {
			switch (shaderPack)
			{
			case ComputePipelineState::kDelaunayDiagram:
			default:
#pragma region ドロネー図作成用のシェーダー
			{
				// Descriptor Range の設定
				CD3DX12_DESCRIPTOR_RANGE descriptorRangeSRV(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
				CD3DX12_DESCRIPTOR_RANGE descriptorRangeUAV1(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0);
				CD3DX12_DESCRIPTOR_RANGE descriptorRangeUAV2(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 1);

				// Root Parameter の作成
				CD3DX12_ROOT_PARAMETER rootParameters[3] = {};
				rootParameters[0].InitAsDescriptorTable(1, &descriptorRangeSRV, D3D12_SHADER_VISIBILITY_ALL); // SRV (入力点データ)
				rootParameters[1].InitAsDescriptorTable(1, &descriptorRangeUAV1, D3D12_SHADER_VISIBILITY_ALL); // UAV (出力三角形データ)
				rootParameters[2].InitAsDescriptorTable(1, &descriptorRangeUAV2, D3D12_SHADER_VISIBILITY_ALL); // UAV (カウンターバッファ)

				// Sampler の設定
				CD3DX12_STATIC_SAMPLER_DESC staticSampler(
					0, D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_WRAP,
					D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP
				);

				// Root Signature の作成
				CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
				rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 1, &staticSampler, D3D12_ROOT_SIGNATURE_FLAG_NONE);

				// シリアライズと作成
				HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob[shaderPack], &errorBlob[shaderPack]);
				if (FAILED(hr)) {
					std::cerr << "Root Signature Serialization Failed: " << reinterpret_cast<char*>(errorBlob[shaderPack]->GetBufferPointer()) << std::endl;
					assert(false);
				}

				hr = directXCommon->GetDevice()->CreateRootSignature(0, signatureBlob[shaderPack]->GetBufferPointer(), signatureBlob[shaderPack]->GetBufferSize(), IID_PPV_ARGS(&rootSignature_[kDelaunayDiagram]));
				assert(SUCCEEDED(hr));
			}
#pragma endregion
			break;
			}
		}
	}

	void ComputePipelineManager::CreatePSO() {
		DirectXCommon* directXCommon = DirectXCommon::GetInstance();

		// Shaderのコンパイル
		ComPtr<IDxcBlob> computeShaderBlob[ComputePipelineState::kCountOfComputePipelineState] = { 0 };
		for (int shaderPack = 0; shaderPack < ComputePipelineState::kCountOfComputePipelineState; shaderPack++) {
			switch (shaderPack)
			{
			case ComputePipelineState::kDelaunayDiagram:
			default:
				// コンピュートシェーダー
				computeShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/DelaunayDiagram.CS.hlsl", L"cs_6_0");
				assert(computeShaderBlob[shaderPack] != nullptr);
			}
		}

		// PSOの生成
		D3D12_COMPUTE_PIPELINE_STATE_DESC computePipeLineStateDesc = {};
		for (int shaderPack = 0; shaderPack < ComputePipelineState::kCountOfComputePipelineState; shaderPack++) {
			for (int blendMode = 0; blendMode < BlendMode::kCountOfBlendMode; blendMode++) {
				computePipeLineStateDesc.pRootSignature = rootSignature_[shaderPack].Get();
				computePipeLineStateDesc.CS = { computeShaderBlob[shaderPack]->GetBufferPointer(), computeShaderBlob[shaderPack]->GetBufferSize() };
				computePipeLineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
				computePipeLineStateDesc.NodeMask = 0;

				// 実際に生成
				[[maybe_unused]] LRESULT hr = directXCommon->GetDevice()->CreateComputePipelineState(&computePipeLineStateDesc, IID_PPV_ARGS(&computePipelineState_[shaderPack]));
				assert(SUCCEEDED(hr));
			}
		}
	}

}