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

		static std::shared_ptr<Model> Create(const std::string& filename);

		static std::shared_ptr<Model> Create(const std::string& filepath, const std::string& filename);

	private: //静的メンバ変数

		static std::map<std::string, std::shared_ptr<Model>> sModels_;

	public: //メンバ関数
		Model();
		~Model();

		void Initialize(const std::string& filepath, const std::string& filename);

		void Draw(RenderItem& renderItem);
		void Draw(RenderItem& renderItem, uint32_t textureHandle);

		void Draw(const ParticleDrawInfo& drawInfo);
		void Draw(const ParticleDrawInfo& drawInfo, uint32_t textureHandle);

	public: //ゲッター

		inline const std::list<AnimationData> GetAnimationData() { return animations_; }

		inline const RootNode GetInialNode() { return rootNode_; }

		inline const Skeleton GetSkeleton() { return skeleton_; }

		inline const std::list<Mesh> GetMeshs() { return meshs_; }

	private: //メンバ関数

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