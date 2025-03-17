#pragma once
#include <array>
#include <d3d12.h>
#include <wrl.h>
#include <cassert>
#include <iostream>
#include "Base/WinApp/WinApp.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Data/BlendMode.h"

/// <summary>
/// GraphicsPipelineManager.h
/// PSOを管理しているマネージャークラス
/// </summary>

enum ComputePipelineState {
	//ドロネー図作成
	kDelaunayDiagram,
	//パイプラインステートの数
	kCountOfComputePipelineState
};

namespace MyEngine {

	class ComputePipelineManager {
	public: //静的関数

		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns></returns>
		static ComputePipelineManager* GetInstance();

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
		inline ID3D12RootSignature* GetRootSignature(ComputePipelineState shaderPack) { return rootSignature_[shaderPack].Get(); }

		/// <summary>
		/// PSOの取得
		/// </summary>
		/// <param name="shaderPack"></param>
		/// <returns></returns>
		inline ID3D12PipelineState* GetPSO(ComputePipelineState shaderPack) { return computePipelineState_[shaderPack].Get(); }

	private:
		ComputePipelineManager() = default;
		~ComputePipelineManager() = default;

		/// <summary>
		/// パイプラインの生成
		/// </summary>
		void CreateComputePipelineManager();

		/// <summary>
		///	ルートシグネチャーの生成
		/// </summary>
		void CreateRootSignature();

		/// <summary>
		/// PSOの生成
		/// </summary>
		void CreatePSO();

	private: //メンバ変数
		ComPtr<ID3D12RootSignature> rootSignature_[kCountOfComputePipelineState];
		ComPtr<ID3D12PipelineState> computePipelineState_[kCountOfComputePipelineState];
	};

}