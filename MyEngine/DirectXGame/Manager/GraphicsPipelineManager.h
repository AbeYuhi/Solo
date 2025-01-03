#pragma once
#include <array>
#include "Base/WinApp/WinApp.h"
#include "Base/DirectXCommon/DirectXCommon.h"

/// <summary>
/// GraphicsPipelineManager.h
/// PSOを管理しているマネージャークラス
/// </summary>

enum BlendMode{
	//ブレンドなし
	kBlendModeNone,
	//通常ブレンド
	kBlendModeNormal,
	//加算ブレンド
	kBlendModeAdd,
	//減算ブレンド
	kBlendModeSubtract,
	//乗算ブレンド
	kBlendModeMultily,
	//スクリーンブレンド
	kBlendModeScreen,
	//ブレンドモードの数(絶対に使うな)
	kCountOfBlendMode,
};

enum PipelineState {
	//通常用
	kDefault,
	//スプライト用
	kSprite,
	//スカイボックス用
	kSkyBox,
	//パーティクル用
	kParticle,
	//ワイヤフレーム用
	kWireFrame,
	//スキニング用
	kSkinning,
	//パイプラインステートの数
	kCountOfPipelineState
};

namespace MyEngine {

	class GraphicsPipelineManager {
	public: //静的関数
		static GraphicsPipelineManager* GetInstance();

	public: //メンバ関数

		void Initialize();

	public: //ゲッターセッター

		inline ID3D12RootSignature* GetRootSignature(PipelineState shaderPack = kDefault) { return rootSignature_[shaderPack].Get(); }
		inline ID3D12PipelineState* GetPSO(PipelineState shaderPack = kDefault) { return graphicsPipelineState_[shaderPack][blendMode_].Get(); }
		inline D3D12_VIEWPORT* GetViewPort() { return &viewPort_; }
		inline D3D12_RECT* GetScissorRect() { return &scissorRect_; }
		inline void SetBlendMode(BlendMode blendMode) { blendMode_ = blendMode; }
		inline BlendMode GetBlendMode() { return blendMode_; }

	private:
		GraphicsPipelineManager() = default;
		~GraphicsPipelineManager() = default;

		void CreateGraphicsPipelineManager();

		/// <summary>
		///	ルートシグネチャーの生成
		/// </summary>
		void CreateRootSignature();

		/// <summary>
		/// PSOの生成
		/// </summary>
		void CreatePSO();

	private: //メンバ変数
		ComPtr<ID3D12RootSignature> rootSignature_[kCountOfPipelineState];
		ComPtr<ID3D12PipelineState> graphicsPipelineState_[kCountOfPipelineState][BlendMode::kCountOfBlendMode];
		D3D12_VIEWPORT viewPort_;
		D3D12_RECT scissorRect_;
		BlendMode blendMode_;
	};

}