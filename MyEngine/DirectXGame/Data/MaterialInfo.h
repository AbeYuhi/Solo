#pragma once
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Base/CreateResource/CreateResource.h"
#include "Transform.h"
#include "Material.h"
#include "MaterialData.h"
#include "BlendMode.h"

/// <summary>
/// MaterialInfo.h
/// マテリアルをまとめているファイル
/// </summary>

namespace MyEngine {

	struct MaterialInfo {
		//Resource
		ComPtr<ID3D12Resource> resource_ = nullptr;
		//UVtransform
		EulerTransformData uvTransform_;
		//マテリアル情報
		Material* material_;
		//非表示かどうか
		bool isInvisible_;
		//環境マッピングのテクスチャハンドル
		uint32_t environmentTextureHandle_;
		//ブレンドモード
		BlendMode blendMode_;

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// アフィン変換
		/// </summary>
		void UpdateMatrix();

	private:
		/// <summary>
		/// リソースの生成
		/// </summary>
		void CreateResource();

		/// <summary>
		/// MAPの実行
		/// </summary>
		void RunMap();
	};

	struct ParticleMaterialInfo {
		//Resource
		ComPtr<ID3D12Resource> resource_ = nullptr;
		//UVtransform
		EulerTransformData uvTransform_;
		//マテリアル情報
		ParticleMaterial* material_;
		//非表示かどうか
		bool isInvisible_;
		//ブレンドモード
		BlendMode blendMode_;

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// アフィン変換
		/// </summary>
		void UpdateMatrix();

	private:
		/// <summary>
		/// リソースの生成
		/// </summary>
		void CreateResource();

		/// <summary>
		/// MAPの実行
		/// </summary>
		void RunMap();
	};

}