#pragma once
#include <string>
#include <vector>
#include <list>
#include <map>
#include <chrono>
#include "Math/Math.h"
#include "AnimationData.h"
#include "Node.h"
#include "Skeleton.h"
#include "SkinCluster.h"

/// <summary>
/// Animation.h
/// アニメーションの情報すべてをまとめるための構造体があるファイル
/// </summary>

namespace MyEngine {

	class Model;

	struct Animation {
		//アニメーションのローカル情報
		std::vector<AnimationInfo> infos;
		//Node情報
		RootNode rootNode;
		//初期モデルのNode
		RootNode initialNode;
		//Skeleton情報
		Skeleton skeleton;
		Skeleton initialSkeleton;
		//SkinCluster情報
		std::map<std::string, SkinCluster> skinClusters;
		std::vector<std::string> meshNames;
		std::chrono::steady_clock::time_point preTime;

		/// <summary>
		/// 初期化処理
		/// </summary>
		void Initialize();

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();

		/// <summary>
		/// Nodeの更新処理
		/// </summary>
		/// <param name="info"></param>
		void NodeUpdate(AnimationInfo& info);

		/// <summary>
		/// スケルトンの更新処理
		/// </summary>
		void SkeletonUpdate();

		/// <summary>
		/// アニメーションの再生処理
		/// </summary>
		/// <param name="info"></param>
		void ApplyAnimation(AnimationInfo& info);

		/// <summary>
		/// スキンクラスターの更新
		/// </summary>
		void SkinClusterUpdate();

		/// <summary>
		/// アニメーションをセットする処理
		/// </summary>
		/// <param name="datas"></param>
		void SetAnimation(std::list<AnimationData> datas);

		/// <summary>
		/// スケルトンを描画する処理
		/// </summary>
		void SkeletonDraw();

		/// <summary>
		/// Modelをセットする処理
		/// </summary>
		/// <param name="model"></param>
		void SetModel(Model* model);

		/// <summary>
		/// アニメーションの再生スピードを調整する処理
		/// </summary>
		/// <param name="animationName"></param>
		/// <param name="speed"></param>
		void SetAnimationSpeed(std::string animationName, float speed);

	};

}