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
#include "GameObject/Entity/Wall.h"
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
	ModelDrawInfo info;
	ObjectType type;
	Collider collider;
};

struct LevelObject {
	std::vector<std::unique_ptr<ObjData>> objDatas_;
	std::vector<Wall> wallDatas_;
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

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="fileName"></param>
	/// <param name="stageSize"></param>
	void Initialize(const std::string& fileName, float stageSize = 0.0f);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 削除処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// カメラデータの取得処理
	/// </summary>
	/// <returns></returns>
	inline CameraData GetCameraData() const { return gameObject_.cameraData_; }

	/// <summary>
	/// クリスタルデータの取得処理
	/// </summary>
	/// <returns></returns>
	inline std::vector<Crystal>& GetCrystals() { return gameObject_.crystalDatas_; }

	/// <summary>
	/// Glassオブジェクトへの参照を返す
	/// </summary>
	/// <returns>Glassの参照のベクター</returns>
	inline std::vector<Glass*> GetGlasses() const {
		std::vector<Glass*> result;
		result.reserve(gameObject_.glassDatas_.size());
		for (const auto& uptr : gameObject_.glassDatas_) {
			result.push_back(uptr.get());
		}
		return result;
	}

private: //メンバ関数

	/// <summary>
	/// ステージファイルの読み込み処理
	/// </summary>
	/// <param name="fileName"></param>
	void LoadFile(const std::string& fileName);

	/// <summary>
	/// オブジェクトの子を読み込む処理
	/// </summary>
	/// <param name="levelData"></param>
	/// <param name="childrens"></param>
	/// <param name="parent"></param>
	void ScanChildData(LevelData* levelData, json& childrens, int32_t parent);

	/// <summary>
	/// レベルを作成する処理
	/// </summary>
	void LevelCreate();

private: //メンバ変数
	//グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/ScriptFilles/";
	std::unique_ptr<LevelData> levelData_;

	uint32_t wallTexutre_;

	EulerTransformData levelSceneData_;
	LevelObject gameObject_;

};

