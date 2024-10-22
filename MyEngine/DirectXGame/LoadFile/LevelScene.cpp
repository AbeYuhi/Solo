#include "LevelScene.h"

void LevelScene::Initialize(std::string fileName, unsigned int stageNum) {

	levelSceneData_.Initialize();
	levelSceneData_.translate_ = {0.0f, 0.0f, stageNum * 100.0f};

	//ステージの読み込み
	LoadFile(fileName);

	//ステージの生成
	LevelCreate();
}

void LevelScene::Update() {

#ifdef _DEBUG
	ImGui::Begin("levelObjInfo");
	ImGui::BeginTabBar("levelObjInfo");
	for (auto& levelObject : gameObject_.objDatas_) {
		ImGuiManager::GetInstance()->RenderItemDebug(levelObject->objName + "info", levelObject->renderItem);
	}
	ImGui::EndTabBar();
	ImGui::End();
#endif // _DEBUG

	for (auto& crystalData : gameObject_.crystalDatas_) {
		crystalData.Update();
	}
	for (auto& doorData : gameObject_.doorDatas_) {
		doorData.Update();
	}
}

void LevelScene::Draw() {

	for (auto& levelObject : gameObject_.wallDatas_) {
		levelObject.model->Draw(levelObject.renderItem, 1);
	}
	for (auto& crystalData : gameObject_.crystalDatas_) {
		crystalData.Draw();
	}
	for (auto& doorData : gameObject_.doorDatas_) {
		doorData.Draw();
	}
}

void LevelScene::LoadFile(std::string fileName) {
	const std::string fullPath = kDirectoryPath + fileName;

	//ファイルストリーム
	std::ifstream file;

	//ファイルを開く
	file.open(fullPath);

	//ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	//JSON文字列から解凍したデータ
	json deserialized;
	//解凍
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	//正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	//レベルデータ格納用インスタンス
	levelData_ = std::make_unique<LevelData>();

	//"objects"の全オブジェクトを走査
	for (auto& object : deserialized["objects"]) {
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		//種類ごとの処理

		//Mesh
		if (type.compare("MESH") == 0) {
			//要素追加
			levelData_->objects.emplace_back(LevelData::ObjectData{});
			//今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData_->objects.back();

			if (object.contains("name")) {
				objectData.objName = object["name"];
			}

			if (object.contains("file_name")) {
				objectData.fileName = object["file_name"];
			}

			if (object.contains("draw_check")) {
				objectData.drawCheck = object["draw_check"];
			}
			else {
				objectData.drawCheck = true;
			}

			objectData.type = kMESH;

			//トランスフォーム
			json& transform = object["transform"];
			//平行移動
			objectData.translation.x = (float)transform["translation"][0];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = (float)transform["translation"][1];
			//回転角
			objectData.rotation.x = -(float)transform["rotation"][0] * (3.14f / 180.0f);
			objectData.rotation.y = -(float)transform["rotation"][2] * (3.14f / 180.0f);
			objectData.rotation.z = -(float)transform["rotation"][1] * (3.14f / 180.0f);
			//スケーリング
			objectData.scaling.x = (float)transform["scaling"][0];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][1];

			if (object.contains("collider")) {
				json& collider = object["collider"];
				//コライダータイプ
				LevelData::ObjectCollider colliderData;
				colliderData.type = collider["type"];
				colliderData.tag = collider["tag"];

				if (colliderData.type == "AABB") {
					//ポジション
					colliderData.centerPos.x = collider["center"][0];
					colliderData.centerPos.y = collider["center"][2];
					colliderData.centerPos.z = collider["center"][1];
					//回転
					colliderData.rotate.x = 0.0f;
					colliderData.rotate.y = 0.0f;
					colliderData.rotate.z = 0.0f;
					//サイズ
					colliderData.size.x = collider["size"][0];
					colliderData.size.y = collider["size"][2];
					colliderData.size.z = collider["size"][1];
				}
				else if (colliderData.type == "OBB") {
					//ポジション
					colliderData.centerPos.x = collider["center"][0];
					colliderData.centerPos.y = collider["center"][2];
					colliderData.centerPos.z = collider["center"][1];
					//回転
					colliderData.rotate.x = -1 * collider["rotate"][0] * (3.14f / 180.0f);
					colliderData.rotate.y = -1 * collider["rotate"][2] * (3.14f / 180.0f);
					colliderData.rotate.z = -1 * collider["rotate"][1] * (3.14f / 180.0f);
					//サイズ
					colliderData.size.x = collider["size"][0];
					colliderData.size.y = collider["size"][2];
					colliderData.size.z = collider["size"][1];
				}
				else if (colliderData.type == "Sphere") {

					//ポジション
					colliderData.centerPos.x = collider["center"][0];
					colliderData.centerPos.y = collider["center"][2];
					colliderData.centerPos.z = collider["center"][1];
					//サイズ
					colliderData.radius = collider["radius"];
				}

				if (colliderData.tag == "GLASS") {
					

					colliderData.glassInfo.groundingInfosUp = collider["groundingInfos_up"];
					colliderData.glassInfo.groundingInfosDown = collider["groundingInfos_down"];
					colliderData.glassInfo.groundingInfosRight = collider["groundingInfos_right"];
					colliderData.glassInfo.groundingInfosLeft = collider["groundingInfos_left"];

					colliderData.glassInfo.moveType = collider["glassMoveTypes"];

					colliderData.glassInfo.verticalDivisions = collider["verticalDivisions"];
					colliderData.glassInfo.horizontalDivisions = collider["horizontalDivisions"];
				}

				colliderData.collisionCheck = collider["collision_check"];
				colliderData.tag = collider["tag"];
				objectData.collider = colliderData;
			}

			if (object.contains("children")) {
				ScanChildData(levelData_.get(), object, static_cast<int32_t>(levelData_->objects.size()) - 1);
			}
		}

		//Light


		//Camera
		if (type.compare("CAMERA") == 0) {
			//要素追加
			levelData_->objects.emplace_back(LevelData::ObjectData{});
			//今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData_->objects.back();

			objectData.type = kCamera;

			//トランスフォーム
			json& transform = object["transform"];
			//平行移動
			objectData.translation.x = (float)transform["translation"][0];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = (float)transform["translation"][1];
			//回転角
			objectData.rotation.x = -((float)transform["rotation"][0] - 90.0f) * (3.14f / 180.0f);
			objectData.rotation.y = -(float)transform["rotation"][2] * (3.14f / 180.0f);
			objectData.rotation.z = -(float)transform["rotation"][1] * (3.14f / 180.0f);
			//スケーリング
			objectData.scaling.x = (float)transform["scaling"][0];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][1];

			if (object.contains("collider")) {
				json& collider = object["collider"];
				//コライダータイプ
				LevelData::ObjectCollider colliderData;
				colliderData.type = collider["type"];
				if (colliderData.type == "AABB") {
					//ポジション
					colliderData.centerPos.x = collider["center"][0];
					colliderData.centerPos.y = collider["center"][2];
					colliderData.centerPos.z = collider["center"][1];
					//回転
					colliderData.rotate.x = 0.0f;
					colliderData.rotate.y = 0.0f;
					colliderData.rotate.z = 0.0f;
					//サイズ
					colliderData.size.x = collider["size"][0];
					colliderData.size.y = collider["size"][2];
					colliderData.size.z = collider["size"][1];
				}
				else if (colliderData.type == "OBB") {
					//ポジション
					colliderData.centerPos.x = collider["center"][0];
					colliderData.centerPos.y = collider["center"][2];
					colliderData.centerPos.z = collider["center"][1];
					//回転
					colliderData.rotate.x = -1 * collider["rotate"][0] * (3.14f / 180.0f);
					colliderData.rotate.y = -1 * collider["rotate"][2] * (3.14f / 180.0f);
					colliderData.rotate.z = -1 * collider["rotate"][1] * (3.14f / 180.0f);
					//サイズ
					colliderData.size.x = collider["size"][0];
					colliderData.size.y = collider["size"][2];
					colliderData.size.z = collider["size"][1];
				}
				else if (colliderData.type == "Sphere") {

					//ポジション
					colliderData.centerPos.x = collider["center"][0];
					colliderData.centerPos.y = collider["center"][2];
					colliderData.centerPos.z = collider["center"][1];
					//サイズ
					colliderData.radius = collider["radius"];
				}
				colliderData.collisionCheck = collider["collision_check"];
				colliderData.tag = collider["tag"];
				objectData.collider = colliderData;
			}
		}
	}
}

void LevelScene::ScanChildData(LevelData* levelData, json& childrens, int32_t parent) {
	for (auto& object : childrens["children"]) {
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		//種類ごとの処理
		//Mesh
		if (type.compare("MESH") == 0) {
			//要素追加
			levelData_->objects.emplace_back(LevelData::ObjectData{});
			//今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData_->objects.back();

			if (object.contains("name")) {
				objectData.objName = object["name"];
			}

			if (object.contains("file_name")) {
				objectData.fileName = object["file_name"];
			}

			if (object.contains("draw_check")) {
				objectData.drawCheck = object["draw_check"];
			}

			objectData.type = kMESH;

			//トランスフォーム
			json& transform = object["transform"];
			//平行移動
			objectData.translation.x = (float)transform["translation"][0];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = (float)transform["translation"][1];
			//回転角
			objectData.rotation.x = -(float)transform["rotation"][0] * (3.14f / 180.0f);
			objectData.rotation.y = -(float)transform["rotation"][2] * (3.14f / 180.0f);
			objectData.rotation.z = -(float)transform["rotation"][1] * (3.14f / 180.0f);
			//スケーリング
			objectData.scaling.x = (float)transform["scaling"][0];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][1];

			objectData.parent = parent;

			if (object.contains("collider")) {
				json& collider = object["collider"];
				//コライダータイプ
				LevelData::ObjectCollider colliderData;
				colliderData.type = collider["type"];
				colliderData.tag = collider["tag"];
				if (colliderData.type == "AABB") {
					//ポジション
					colliderData.centerPos.x = collider["center"][0];
					colliderData.centerPos.y = collider["center"][2];
					colliderData.centerPos.z = collider["center"][1];
					//回転
					colliderData.rotate.x = 0.0f;
					colliderData.rotate.y = 0.0f;
					colliderData.rotate.z = 0.0f;
					//サイズ
					colliderData.size.x = collider["size"][0];
					colliderData.size.y = collider["size"][2];
					colliderData.size.z = collider["size"][1];
				}
				else if (colliderData.type == "OBB") {
					//ポジション
					colliderData.centerPos.x = collider["center"][0];
					colliderData.centerPos.y = collider["center"][2];
					colliderData.centerPos.z = collider["center"][1];
					//回転
					colliderData.rotate.x = -1 * collider["rotate"][0] * (3.14f / 180.0f);
					colliderData.rotate.y = -1 * collider["rotate"][2] * (3.14f / 180.0f);
					colliderData.rotate.z = -1 * collider["rotate"][1] * (3.14f / 180.0f);
					//サイズ
					colliderData.size.x = collider["size"][0];
					colliderData.size.y = collider["size"][2];
					colliderData.size.z = collider["size"][1];
				}
				else if (colliderData.type == "Sphere") {

					//ポジション
					colliderData.centerPos.x = collider["center"][0];
					colliderData.centerPos.y = collider["center"][2];
					colliderData.centerPos.z = collider["center"][1];
					//サイズ
					colliderData.radius = collider["radius"];
				}

				if (colliderData.tag == "GLASS") {
					colliderData.glassInfo.groundingInfosUp = collider["groundingInfos_up"];
					colliderData.glassInfo.groundingInfosDown = collider["groundingInfos_down"];
					colliderData.glassInfo.groundingInfosRight = collider["groundingInfos_right"];
					colliderData.glassInfo.groundingInfosLeft = collider["groundingInfos_left"];

					colliderData.glassInfo.moveType = collider["glassMoveTypes"];

					colliderData.glassInfo.verticalDivisions = collider["verticalDivisions"];
					colliderData.glassInfo.horizontalDivisions = collider["horizontalDivisions"];
				}

				colliderData.collisionCheck = collider["collision_check"];
				colliderData.tag = collider["tag"];
				objectData.collider = colliderData;
			}

			if (object.contains("children")) {
				ScanChildData(levelData, object["children"], static_cast<int32_t>(levelData_->objects.size()) - 1);
			}
		}
	}
}

void LevelScene::LevelCreate() {

	//レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData_->objects) {
		std::unique_ptr<ObjData> levelObject = std::make_unique<ObjData>();
		if (objectData.type == kMESH) {

			levelObject->renderItem.Initialize();
			levelObject->renderItem.worldTransform_.data_.translate_ = objectData.translation + levelSceneData_.translate_;
			levelObject->renderItem.worldTransform_.data_.rotate_ = objectData.rotation;
			levelObject->renderItem.worldTransform_.data_.scale_ = objectData.scaling;
			levelObject->model = Model::Create(objectData.fileName);
			levelObject->objName = objectData.objName;
			levelObject->renderItem.materialInfo_.material_->enableLightint = 1;
			levelObject->type = kMESH;

			if (objectData.drawCheck) {
				levelObject->renderItem.materialInfo_.isInvisible_ = false;
			}
			else {
				levelObject->renderItem.materialInfo_.isInvisible_ = true;
			}

			if (objectData.parent) {
				levelObject->renderItem.worldTransform_.parent_ = &gameObject_.objDatas_[*objectData.parent]->renderItem.worldTransform_;
			}
		}
		else if (objectData.type == kCamera) {
			levelObject->renderItem.Initialize();
			levelObject->renderItem.worldTransform_.data_.translate_ = objectData.translation + levelSceneData_.translate_;
			levelObject->renderItem.worldTransform_.data_.rotate_ = objectData.rotation;
			levelObject->renderItem.worldTransform_.data_.scale_ = objectData.scaling;
			gameObject_.cameraData_ = { .CameraInfo = levelObject->renderItem.worldTransform_.data_, .cameraSpeed = 1.0f };
			levelObject->type = kCamera;
		}

		if (objectData.collider) {
			ColliderType type = kAABB;
			ColliderTag tag = WALL;
			if (objectData.collider->type == "AABB") {
				type = kAABB;
			}
			else if (objectData.collider->type == "OBB") {
				type = kOBB;
			}
			else if (objectData.collider->type == "SPHERE") {
				type = kSPHERE;
			}

			if (objectData.collider->tag == "WALL") {
				tag = WALL;
			}
			else if (objectData.collider->tag == "GOAL") {
				tag = GOAL;
			}
			else if (objectData.collider->tag == "CRYSTAL") {
				tag = CRYSTAL;
			}
			else if (objectData.collider->tag == "BUTTON") {
				tag = BUTTON;
			}
			else if (objectData.collider->tag == "LDOOR") {
				tag = LDOOR;
			}
			else if (objectData.collider->tag == "RDOOR") {
				tag = RDOOR;
			}
			else if (objectData.collider->tag == "GLASS") {
				tag = GLASS;
			}

			if (objectData.collider->type != "NONE" && objectData.collider->type != "GLASS") {

				if (objectData.type != kCamera) {
					levelObject->collider.Initialize(
						levelObject->renderItem.worldTransform_.GetPEulerTransformData(),
						{ .scale_ = objectData.collider->size, .rotate_ = objectData.collider->rotate, .translate_ = objectData.collider->centerPos },
						tag,
						type,
						objectData.collider->collisionCheck);

					CollisionManager::GetInstance()->AddCollider(&levelObject->collider);
				}
			}
		}
		
		if (objectData.collider) {
			if (objectData.collider->tag == "BUTTON") {
				Door door;
				door.Initialize(levelObject->model, &levelObject->renderItem, &levelObject->collider);
				gameObject_.doorDatas_.push_back(door);
			}
			else if (objectData.collider->tag == "LDOOR") {
				gameObject_.doorDatas_[gameObject_.doorDatas_.size() - 1].SetLeftDoor(levelObject->model, &levelObject->renderItem, &levelObject->collider);
			}
			else if (objectData.collider->tag == "RDOOR") {
				gameObject_.doorDatas_[gameObject_.doorDatas_.size() - 1].SetRightDoor(levelObject->model, &levelObject->renderItem, &levelObject->collider);
			}
			else if (objectData.collider->tag == "CRYSTAL") {
				Crystal crystal;
				crystal.Initialize(levelObject->model, &levelObject->renderItem, &levelObject->collider);
				gameObject_.crystalDatas_.push_back(crystal);
			}
			else if (objectData.collider->tag == "GLASS") {
				Glass glass;
				glass.Initialize(levelObject->model, &levelObject->renderItem, objectData.collider->glassInfo);
				gameObject_.glassDatas_.push_back(glass);
			}
			else{
				if (objectData.type == kMESH) {
					gameObject_.wallDatas_.push_back(*levelObject);
				}
			}
		}
		else {
			if (objectData.type == kMESH) {
				gameObject_.wallDatas_.push_back(*levelObject);
			}
		}

		gameObject_.objDatas_.push_back(std::move(levelObject));
	}
}