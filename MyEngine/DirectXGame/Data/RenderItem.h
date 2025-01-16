#pragma once
#include <vector>
#include "Animation.h"
#include "MaterialInfo.h"
#include "Transform.h"
#include "Mesh.h"

/// <summary>
/// RenderItem.h
/// レンダリングに用いる情報を収納する構造体ファイル
/// </summary>

namespace MyEngine {

	class Model;

	struct RenderItem {
		WorldTransform worldTransform_;
		std::vector<WorldTransform> meshWorldTransforms_;
		MaterialInfo materialInfo_;
		Animation animation_;

		~RenderItem();

		/// <summary>
		/// レンダリング情報の初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();

		/// <summary>
		/// GLTFの更新処理
		/// </summary>
		/// <param name="mesh"></param>
		/// <param name="index"></param>
		void UpdateGltf(Mesh mesh, int index);

		/// <summary>
		/// Modekのセッター
		/// </summary>
		/// <param name="model"></param>
		void SetModel(Model* model);

		/// <summary>
		/// アニメーションのセッター
		/// </summary>
		/// <param name="datas"></param>
		void SetAnimation(std::list<AnimationData> datas);
	};

}