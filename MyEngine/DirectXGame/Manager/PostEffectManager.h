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

		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns></returns>
		static PostEffectManager* GetInstance();

	public: //メンバ関数

		/// <summary>
		/// 初期化処理
		/// </summary>
		void Initialize();

		/// <summary>
		/// 描画前処理
		/// </summary>
		void PreDraw();

		/// <summary>
		/// 描画後処理
		/// </summary>
		void PostDraw();

	public: //ゲッターセッター

		/// <summary>
		/// ポストエフェクトのゲッターセッター
		/// </summary>
		/// <returns></returns>
		inline PostEffect GetPostEffect() { return postEffect_; }
		inline void SetPostEffect(PostEffect postEffect) { postEffect_ = postEffect; }

		/// <summary>
		/// smoothingの時のカーネルのゲッターセッター
		/// </summary>
		/// <returns></returns>
		inline int32_t GetKernelSize() { return smoothingInfo_->kernelSize; }
		inline void SetKernelSize(int32_t kernelSize) { smoothingInfo_->kernelSize = kernelSize; }

		/// <summary>
		/// smoothingのタイプのゲッターセッター
		/// </summary>
		/// <returns></returns>
		inline int32_t GetSmoothingType() { return smoothingInfo_->type; }
		inline void SetSmoothingType(int32_t type) { smoothingInfo_->type = type; }

		/// <summary>
		/// smoothingのブラーの強さのゲッターセッター
		/// </summary>
		/// <returns></returns>
		inline float GetBlurStrength() { return smoothingInfo_->blurStrength; }
		inline void SetBlurStrength(float blurStrength) { smoothingInfo_->blurStrength = blurStrength; }

		/// <summary>
		/// HSVマテリアルの取得
		/// </summary>
		/// <returns></returns>
		inline HSVMaterial* GetHSVMaterial() { return hsvMaterial_; }

		/// <summary>
		/// radialBlur情報の取得
		/// </summary>
		/// <returns></returns>
		inline RadialBlurInfo* GetRadialBlurInfo() { return radialBlurInfo_; }

		/// <summary>
		/// vignetteBlur情報の取得
		/// </summary>
		/// <returns></returns>
		inline VignetteBlurInfo* GetVignetteBlurInfo() { return vignetteBlurInfo_; }

	private:
		PostEffectManager() = default;
		~PostEffectManager() = default;

		/// <summary>
		/// レンダーテクスチャの作成
		/// </summary>
		void CreateRenderTexture();

		/// <summary>
		/// ルートシグネチャーの作成
		/// </summary>
		void CreateRootSignature();

		/// <summary>
		/// PSOの作成
		/// </summary>
		void CreatePSO();

		/// <summary>
		/// 非ポストエフェクト時の描画前処理
		/// </summary>
		void NormalPreDraw();

		/// <summary>
		/// ポストエフェクト時の描画前処理
		/// </summary>
		void RenderPreDraw();

		/// <summary>
		/// 描画後処理
		/// </summary>
		void RenderPostDraw();

		/// <summary>
		/// レンダーテクスチャーのリソースを作成する処理
		/// </summary>
		/// <param name="width"></param>
		/// <param name="height"></param>
		/// <param name="format"></param>
		/// <param name="clearColor"></param>
		/// <returns></returns>
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