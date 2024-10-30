#pragma once
#include <variant>
#include <map>
#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <optional>
#include "External/nlohmann/json.hpp"
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Manager/ImGuiManager.h"
#include "DirectXGame/Manager/CollisionManager.h"
#include "GameObject/Camera/MainCamera.h"
#include "GameObject/Camera/InGameCamera.h"
#include "GameObject/Entity/Door.h"
#include "GameObject/Entity/Crystal.h"
#include "GameObject/Entity/Glass.h"
#include "Object/Model.h"
#include "Data/LevelData.h"
#include "Data/Collider.h"

/// <summary>
/// LevelScene.h
/// レベルシーンを読み込み描画するクラス
/// </summary>

using json = nlohmann::json;

struct ObjData {
	std::string objName;
	std::shared_ptr<Model> model;
	RenderItem renderItem;
	ObjectType type;
	Collider collider;
};

struct LevelObject {
	std::vector<std::unique_ptr<ObjData>> objDatas_;
	std::vector<ObjData> wallDatas_;
	std::vector<Door> doorDatas_;
	std::vector<Crystal> crystalDatas_;
	std::vector<std::unique_ptr<Glass>> glassDatas_;
	CameraData cameraData_;
};

class LevelScene
{
public:
	LevelScene() = default;
	~LevelScene() = default;

	void Initialize(std::string fileName, unsigned int stageNum = 0);

	void Update();

	void Draw();

	inline CameraData GetCameraData() const { return gameObject_.cameraData_; }
	inline std::vector<Crystal>& GetCrystals() { return gameObject_.crystalDatas_; }

private: //メンバ関数

	void LoadFile(std::string fileName);

	void ScanChildData(LevelData* levelData, json& childrens, int32_t parent);

	void LevelCreate();

private: //メンバ変数
	//グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/ScriptFilles/";
	std::unique_ptr<LevelData> levelData_;

	EulerTransformData levelSceneData_;
	LevelObject gameObject_;

};

