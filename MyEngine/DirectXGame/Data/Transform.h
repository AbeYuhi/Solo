#pragma once
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Math/Matrix4x4.h"
#include "DirectXGame/Math/Quaternion.h"
#include "DirectXGame/Math/Math.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Base/CreateResource/CreateResource.h"

/// <summary>
/// Transform.h
/// シェーダーに送るためのワールド情報構造体が入っているファイル
/// </summary>

struct TransformMatrix {
	Matrix4x4 WVP_;
	Matrix4x4 World_;
	Matrix4x4 WorldInverseTranspose_;
};

namespace MyEngine {

	struct WorldTransform {
		//リソース
		ComPtr<ID3D12Resource> resource_ = nullptr;
		//ローカル情報
		EulerTransformData data_;
		//ワールドMatrix
		Matrix4x4 worldMatrix_;
		//ワールド座標のポジション
		Vector3 worldPos_;
		//Matrix転送済みデータ
		TransformMatrix* matrix_;
		//親のワールドポインタ
		const WorldTransform* parent_ = nullptr;
		//ビュープロジェクションのポインタ
		const Matrix4x4* viewProjectionMatrix_ = nullptr;
		//ワールド空間上でのdata_;
		EulerTransformData worldData_;

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize(bool isSprite);

		/// <summary>
		/// 更新
		/// </summary>
		void UpdateWorld();

		/// <summary>
		/// 転送
		/// </summary>
		void TransferMatrix();

		/// <summary>
		/// Nodeを含めた更新
		/// </summary>
		void NodeUpdate(Matrix4x4 localMatrix);

		/// <summary>
		/// ワールドポジションの取得
		/// </summary>
		Vector3 GetWorldPos();

		/// <summary>
		/// ワールドポジションのポインタ取得
		/// </summary>
		/// <returns></returns>
		Vector3* GetPWorldPos();

		/// <summary>
		/// ローカル座標系でのオブジェクト情報の取得
		/// </summary>
		/// <returns></returns>
		EulerTransformData* GetPLocalEulerTransformData();
		
		/// <summary>
		/// ワールド座標系でのオブジェクト情報の取得
		/// </summary>
		/// <returns></returns>
		EulerTransformData* GetPWorldEulerTransformData();

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