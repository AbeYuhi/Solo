#pragma once
#include <memory>
#include <fstream>
#include <sstream>
#include <map>
#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include "DirectXGame/Base/CreateResource/CreateResource.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Math/Vector2.h"
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Math/Vector4.h"
#include "DirectXGame/Math/Matrix4x4.h"
#include "DirectXGame/Math/Math.h"
#include "DirectXGame/Data/Transform.h"
#include "DirectXGame/Data/ModelData.h"
#include "DirectXGame/Data/Material.h"
#include "DirectXGame/Data/MaterialInfo.h"
#include "DirectXGame/Data/TextureData.h"
#include "DirectXGame/Data/ParticleDrawInfo.h"
#include "DirectXGame/Data/Mesh.h"
#include "DirectXGame/Data/Animation.h"
#include "DirectXGame/Data/Node.h"
#include "DirectXGame/Data/Bone.h"
#include "DirectXGame/Manager/TextureManager.h"
#include "DirectXGame/Manager/GraphicsPipelineManager.h"
#include "DirectXGame/Manager/LightObjectManager.h"
#include "GameObject/Camera/MainCamera.h"

/// <summary>
/// Model.h
/// 3dオブジェクトを表示するためのクラスファイル
/// </summary>

namespace MyEngine {

	struct RenderItem;
	class SkyBox;

	class Model
	{
	public: //静的メンバ関数

		/// <summary>
		/// 3Dモデルの作成関数
		/// </summary>
		/// <param name="filename">ファイルネーム</param>
		/// <returns></returns>
		static std::shared_ptr<Model> Create(const std::string& filename);

		/// <summary>
		/// 3Dモデルの作成関数
		/// </summary>
		/// <param name="filepath">ファイルパス</param>
		/// <param name="filename">ファイルネーム</param>
		/// <returns></returns>
		static std::shared_ptr<Model> Create(const std::string& filepath, const std::string& filename);

	private: //静的メンバ変数

		static std::map<std::string, std::shared_ptr<Model>> sModels_;

	public: //メンバ関数
		Model();
		~Model();

		/// <summary>
		/// 3Dモデルの初期化関数
		/// </summary>
		/// <param name="filepath"></param>
		/// <param name="filename"></param>
		void Initialize(const std::string& filepath, const std::string& filename);

		/// <summary>
		/// 3Dモデルの通常描画処理
		/// </summary>
		/// <param name="renderItem">描画情報</param>
		void Draw(RenderItem& renderItem);
		/// <summary>
		/// 3Dモデルの通常描画処理
		/// </summary>
		/// <param name="renderItem">描画情報</param>
		/// <param name="textureHandle">テクスチャハンドル</param>
		void Draw(RenderItem& renderItem, uint32_t textureHandle);

		/// <summary>
		/// パーティクルの時に使用する描画関数
		/// </summary>
		/// <param name="drawInfo">描画情報</param>
		void Draw(const ParticleDrawInfo& drawInfo);
		/// <summary>
		/// パーティクルの時に使用する描画関数
		/// </summary>
		/// <param name="drawInfo">描画情報</param>
		/// <param name="textureHandle">テクスチャハンドル</param>
		void Draw(const ParticleDrawInfo& drawInfo, uint32_t textureHandle);

	public: //ゲッター

		/// <summary>
		/// アニメーションのデータを取得
		/// </summary>
		/// <returns></returns>
		inline const std::list<AnimationData> GetAnimationData() { return animations_; }

		/// <summary>
		/// 初期状態のノード状態を取得
		/// </summary>
		/// <returns></returns>
		inline const RootNode GetInialNode() { return rootNode_; }

		/// <summary>
		/// スケルトン情報を取得
		/// </summary>
		/// <returns></returns>
		inline const Skeleton GetSkeleton() { return skeleton_; }

		/// <summary>
		/// メッシュデータを取得
		/// </summary>
		/// <returns></returns>
		inline const std::list<Mesh> GetMeshs() { return meshs_; }

	private: //メンバ関数

		/// <summary>
		/// 3Dモデルを他ファイルから読み込む
		/// </summary>
		/// <param name="filepath"></param>
		/// <param name="filename"></param>
		void LoadModelFile(const std::string& filepath, const std::string& filename);

	private: //メンバ変数

		std::string filePath_;
		std::list<Mesh> meshs_;
		std::list<AnimationData> animations_;
		RootNode rootNode_;
		Skeleton skeleton_;

		bool isGltf_;
		bool isBone_;
		bool haveSkininng_;
	};

}