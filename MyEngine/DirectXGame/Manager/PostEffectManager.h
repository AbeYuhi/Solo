#pragma once
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Base/CreateResource/CreateResource.h"
#include "Manager/DrawManager.h"
#include "GameObject/Camera/MainCamera.h"
#include "GraphicsPipelineManager.h"
#include "SrvManager.h"
#include "Data/SmoothingInfo.h"
#include "Data/VignetteBlurInfo.h"
#include "Data/RadialBlurInfo.h"
#include "Data/HSVMaterial.h"
#include "ImGuiManager.h"

/// <summary>
/// PostEffectManager.h
/// ポストエフェクトを管理しているマネージャークラス
/// </summary>

enum PostEffect {
	kNone,
	kCopy,
	kHSVFilter,
	kGrayScale,
	kSepiaScale,
	kVignette,
	kVignetteBlur,
	kSmoothing,
	kRadialBlur,
	kFog,
	kCountOfPostEffect
};

namespace MyEngine {

	class PostEffectManager
	{
	public: //静的関数
		static PostEffectManager* GetInstance();

	public: //メンバ関数

		void Initialize();

		void PreDraw();

		void PostDraw();

	public: //ゲッターセッター

		inline PostEffect GetPostEffect() { return postEffect_; }
		inline void SetPostEffect(PostEffect postEffect) { postEffect_ = postEffect; }

		inline int32_t GetKernelSize() { return smoothingInfo_->kernelSize; }
		inline void SetKernelSize(int32_t kernelSize) { smoothingInfo_->kernelSize = kernelSize; }
		inline int32_t GetSmoothingType() { return smoothingInfo_->type; }
		inline void SetSmoothingType(int32_t type) { smoothingInfo_->type = type; }
		inline float GetBlurStrength() { return smoothingInfo_->blurStrength; }
		inline void SetBlurStrength(float blurStrength) { smoothingInfo_->blurStrength = blurStrength; }

		inline HSVMaterial* GetHSVMaterial() { return hsvMaterial_; }
		inline RadialBlurInfo* GetRadialBlurInfo() { return radialBlurInfo_; }
		inline VignetteBlurInfo* GetVignetteBlurInfo() { return vignetteBlurInfo_; }

	private:
		PostEffectManager() = default;
		~PostEffectManager() = default;

		void CreateRenderTexture();

		void CreateRootSignature();

		void CreatePSO();

		void NormalPreDraw();

		void RenderPreDraw();

		void RenderPostDraw();

		ComPtr<ID3D12Resource>CreateRenderTextureResoruce(uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor);

	private: //メンバ変数
		ComPtr<ID3D12Resource> renderTextureResource_ = nullptr;
		ComPtr<ID3D12RootSignature> rootSignature_[PostEffect::kCountOfPostEffect];
		ComPtr<ID3D12PipelineState> graphicsPipelineState_[PostEffect::kCountOfPostEffect][BlendMode::kCountOfBlendMode];
		PostEffect postEffect_;
		const int kRTVIndex = 2;
		uint32_t srvIndex_;

		//各ポストエフェクトにおける必要なResourceなど
		//HSVFilter 
		ComPtr<ID3D12Resource> hsvMaterialResource_ = nullptr;
		HSVMaterial* hsvMaterial_;
		//Smoothing
		ComPtr<ID3D12Resource> smoothingInfoResource_ = nullptr;
		SmoothingInfo* smoothingInfo_;
		//RadialBlur
		ComPtr<ID3D12Resource> radialBlurInfoResource_ = nullptr;
		RadialBlurInfo* radialBlurInfo_;
		//VignetteBlur
		ComPtr<ID3D12Resource> vignetteBlurInfoResource_ = nullptr;
		VignetteBlurInfo* vignetteBlurInfo_;
		//Fog
		ID3D12Resource* depthTextureResource_;
		uint32_t depthSrvIndex_;
		ComPtr<ID3D12Resource> inverseProjectionResorce_ = nullptr;
		Matrix4x4* inverseProjectionData_;
	};

}