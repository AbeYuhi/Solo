#pragma once
#include <array>
#include "Base/WinApp/WinApp.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Data/BlendMode.h"

/// <summary>
/// GraphicsPipelineManager.h
/// PSOを管理しているマネージャークラス
/// </summary>

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

		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns></returns>
		static GraphicsPipelineManager* GetInstance();

	public: //メンバ関数

		/// <summary>
		/// 初期化関数
		/// </summary>
		void Initialize();

	public: //ゲッターセッター

		/// <summary>
		/// ルートシグネチャーの取得
		/// </summary>
		/// <param name="shaderPack"></param>
		/// <returns></returns>
		inline ID3D12RootSignature* GetRootSignature(PipelineState shaderPack = kDefault) { return rootSignature_[shaderPack].Get(); }
		
		/// <summary>
		/// PSOの取得
		/// </summary>
		/// <param name="shaderPack"></param>
		/// <returns></returns>
		inline ID3D12PipelineState* GetPSO(BlendMode blendMode, PipelineState shaderPack = kDefault) { return graphicsPipelineState_[shaderPack][blendMode].Get(); }
		
		/// <summary>
		/// ビューポートの取得
		/// </summary>
		/// <returns></returns>
		inline D3D12_VIEWPORT* GetViewPort() { return &viewPort_; }

		/// <summary>
		/// シサーレクトの取得
		/// </summary>
		/// <returns></returns>
		inline D3D12_RECT* GetScissorRect() { return &scissorRect_; }

	private:
		GraphicsPipelineManager() = default;
		~GraphicsPipelineManager() = default;

		/// <summary>
		/// パイプラインの生成
		/// </summary>
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
	};

}