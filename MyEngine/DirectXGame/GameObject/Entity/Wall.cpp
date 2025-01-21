#include "Wall.h"

/// <summary>
/// Walls.h
/// 壁に関するクラス関数の実装ファイル
/// </summary>

Wall::Wall(){}
Wall::~Wall(){}

void Wall::Initialize(std::shared_ptr<MyEngine::Model> model,
	std::shared_ptr<MyEngine::RenderItem> renderItem,
	Collider* collider,
	WallInfo info) {

	//アドレスを保存
	data_.model = model;
	data_.renderItem = renderItem;
	data_.collider = collider;
	data_.moveLimit = info.moveLimit;
	data_.moveSpeed = info.moveSpeed;
	data_.rotateSpeed = info.rotateSpeed;
	//MoveTypeをEnumに変換
	if (info.moveType == "DONTMOVE") {
		data_.moveType = DONTMOVE;
	}
	else if (info.moveType == "ALTERNATE_LEFT_RIGHT") {
		data_.moveType = ALTERNATE_LEFT_RIGHT;
	}
	else if (info.moveType == "ALTERNATE_UP_DOWN") {
		data_.moveType = ALTERNATE_UP_DOWN;
	}
	else if (info.moveType == "XROTATE") {
		data_.moveType = XROTATE;
	}
	else if (info.moveType == "YROTATE") {
		data_.moveType = YROTATE;
	}
	else if (info.moveType == "ZROTATE") {
		data_.moveType = ZROTATE;
	}
	else {
		data_.moveType = DONTMOVE;
	}
	//移動前の最初期のデータを保存
	keepData_ = renderItem->worldTransform_.data_;
	//uvTransformのために6面を求める
	Vector3 scale = { std::ceil(data_.renderItem->worldTransform_.data_.scale_.x), std::ceil(data_.renderItem->worldTransform_.data_.scale_.y), std::ceil(data_.renderItem->worldTransform_.data_.scale_.z) };
	for (int i = 0; i < 6; i++) {
		infos_[i].Initialize(model);
		infos_[i].renderItem->worldTransform_.data_ = *data_.renderItem->worldTransform_.GetPWorldEulerTransformData();
	}
	//手前の面
	infos_[0].model = MyEngine::Model::Create("wall", "backWall.obj");
	infos_[0].renderItem->worldTransform_.data_.translate_.z -= data_.renderItem->worldTransform_.data_.scale_.z;
	infos_[0].renderItem->worldTransform_.data_.scale_.z = data_.renderItem->worldTransform_.data_.scale_.y;
	infos_[0].renderItem->materialInfo_.uvTransform_.scale_.x = scale.x;
	infos_[0].renderItem->materialInfo_.uvTransform_.scale_.y = scale.y;
	//奥の面
	infos_[1].model = MyEngine::Model::Create("wall", "frontWall.obj");
	infos_[1].renderItem->worldTransform_.data_.translate_.z += data_.renderItem->worldTransform_.data_.scale_.z;
	infos_[1].renderItem->worldTransform_.data_.scale_.z = data_.renderItem->worldTransform_.data_.scale_.y;
	infos_[1].renderItem->materialInfo_.uvTransform_.scale_.x = scale.x;
	infos_[1].renderItem->materialInfo_.uvTransform_.scale_.y = scale.y;
	//左の面
	infos_[2].model = MyEngine::Model::Create("wall", "leftWall.obj");
	infos_[2].renderItem->worldTransform_.data_.translate_.x -= data_.renderItem->worldTransform_.data_.scale_.x;
	infos_[2].renderItem->worldTransform_.data_.scale_.x = data_.renderItem->worldTransform_.data_.scale_.y;
	infos_[2].renderItem->materialInfo_.uvTransform_.scale_.x = scale.y;
	infos_[2].renderItem->materialInfo_.uvTransform_.scale_.y = scale.z;
	//右の面
	infos_[3].model = MyEngine::Model::Create("wall", "rightWall.obj");
	infos_[3].renderItem->worldTransform_.data_.translate_.x += data_.renderItem->worldTransform_.data_.scale_.x;
	infos_[3].renderItem->worldTransform_.data_.scale_.x = data_.renderItem->worldTransform_.data_.scale_.y;
	infos_[3].renderItem->materialInfo_.uvTransform_.scale_.x = scale.y;
	infos_[3].renderItem->materialInfo_.uvTransform_.scale_.y = scale.z;
	//下の面
	infos_[4].model = MyEngine::Model::Create("wall", "underWall.obj");
	infos_[4].renderItem->worldTransform_.data_.translate_.y -= data_.renderItem->worldTransform_.data_.scale_.y;
	infos_[4].renderItem->materialInfo_.uvTransform_.scale_.x = scale.x;
	infos_[4].renderItem->materialInfo_.uvTransform_.scale_.y = scale.z;
	//上の面
	infos_[5].model = MyEngine::Model::Create("wall", "topWall.obj");
	infos_[5].renderItem->worldTransform_.data_.translate_.y += data_.renderItem->worldTransform_.data_.scale_.y;
	infos_[5].renderItem->materialInfo_.uvTransform_.scale_.x = scale.x;
	infos_[5].renderItem->materialInfo_.uvTransform_.scale_.y = scale.z;
}

void Wall::Update(){

	switch (data_.moveType)
	{
	case Wall::DONTMOVE:
	default:
		DontMove();
		break;
	case Wall::ALTERNATE_LEFT_RIGHT:
		MoveAlternateLeftRight();
		break;
	case Wall::ALTERNATE_UP_DOWN:
		MoveAlternateUpDown();
		break;
	case Wall::XROTATE:
		MoveXRotate();
		break;
	case Wall::YROTATE:
		MoveYRotate();
		break;
	case Wall::ZROTATE:
		MoveZRotate();
		break;
	}

	//スケールの計算
	Vector3 scale = data_.renderItem->worldTransform_.worldData_.scale_;
	Vector3 uvScale = { 0.0f, 0.0f, 0.0f };
	uvScale.x = std::ceil(data_.renderItem->worldTransform_.worldData_.scale_.x);
	uvScale.y = std::ceil(data_.renderItem->worldTransform_.worldData_.scale_.y);
	uvScale.z = std::ceil(data_.renderItem->worldTransform_.worldData_.scale_.z);
	//ワールド情報の更新
	data_.renderItem->worldTransform_.UpdateWorld();
	//6面の移動
	for (int i = 0; i < 6; i++) {
		infos_[i].renderItem->worldTransform_.data_ = *data_.renderItem->worldTransform_.GetPWorldEulerTransformData();
		infos_[i].renderItem->worldTransform_.data_.translate_ = { 0.0f, 0.0f, 0.0f };
	}
	//手前の面
	infos_[0].renderItem->materialInfo_.uvTransform_.scale_.x = uvScale.x;
	infos_[0].renderItem->materialInfo_.uvTransform_.scale_.y = uvScale.y;
	infos_[0].renderItem->worldTransform_.data_.translate_.z -= scale.z;
	infos_[0].renderItem->worldTransform_.data_.translate_ = TransformNormal(infos_[0].renderItem->worldTransform_.data_.translate_, MakeRotateMatrix(data_.renderItem->worldTransform_.GetPWorldEulerTransformData()->rotate_));
	infos_[0].renderItem->worldTransform_.data_.translate_ += data_.renderItem->worldTransform_.GetPWorldEulerTransformData()->translate_;
	//奥の面
	infos_[1].renderItem->materialInfo_.uvTransform_.scale_.x = uvScale.x;
	infos_[1].renderItem->materialInfo_.uvTransform_.scale_.y = uvScale.y;
	infos_[1].renderItem->worldTransform_.data_.translate_.z += scale.z;
	infos_[1].renderItem->worldTransform_.data_.translate_ = TransformNormal(infos_[1].renderItem->worldTransform_.data_.translate_, MakeRotateMatrix(data_.renderItem->worldTransform_.GetPWorldEulerTransformData()->rotate_));
	infos_[1].renderItem->worldTransform_.data_.translate_ += data_.renderItem->worldTransform_.GetPWorldEulerTransformData()->translate_;
	//左の面
	infos_[2].renderItem->materialInfo_.uvTransform_.scale_.x = uvScale.z;
	infos_[2].renderItem->materialInfo_.uvTransform_.scale_.y = uvScale.y;
	infos_[2].renderItem->worldTransform_.data_.translate_.x -= scale.x;
	infos_[2].renderItem->worldTransform_.data_.translate_ = TransformNormal(infos_[2].renderItem->worldTransform_.data_.translate_, MakeRotateMatrix(data_.renderItem->worldTransform_.GetPWorldEulerTransformData()->rotate_));
	infos_[2].renderItem->worldTransform_.data_.translate_ += data_.renderItem->worldTransform_.GetPWorldEulerTransformData()->translate_;
	//右の面
	infos_[3].renderItem->materialInfo_.uvTransform_.scale_.x = uvScale.z;
	infos_[3].renderItem->materialInfo_.uvTransform_.scale_.y = uvScale.y;
	infos_[3].renderItem->worldTransform_.data_.translate_.x += scale.x;
	infos_[3].renderItem->worldTransform_.data_.translate_ = TransformNormal(infos_[3].renderItem->worldTransform_.data_.translate_, MakeRotateMatrix(data_.renderItem->worldTransform_.GetPWorldEulerTransformData()->rotate_));
	infos_[3].renderItem->worldTransform_.data_.translate_ += data_.renderItem->worldTransform_.GetPWorldEulerTransformData()->translate_;
	//下の面
	infos_[4].renderItem->materialInfo_.uvTransform_.scale_.x = uvScale.x;
	infos_[4].renderItem->materialInfo_.uvTransform_.scale_.y = uvScale.z;
	infos_[4].renderItem->worldTransform_.data_.translate_.y -= scale.y;
	infos_[4].renderItem->worldTransform_.data_.translate_ = TransformNormal(infos_[4].renderItem->worldTransform_.data_.translate_, MakeRotateMatrix(data_.renderItem->worldTransform_.GetPWorldEulerTransformData()->rotate_));
	infos_[4].renderItem->worldTransform_.data_.translate_ += data_.renderItem->worldTransform_.GetPWorldEulerTransformData()->translate_;
	//上の面
	infos_[5].renderItem->materialInfo_.uvTransform_.scale_.x = uvScale.x;
	infos_[5].renderItem->materialInfo_.uvTransform_.scale_.y = uvScale.z;
	infos_[5].renderItem->worldTransform_.data_.translate_.y += scale.y;
	infos_[5].renderItem->worldTransform_.data_.translate_ = TransformNormal(infos_[5].renderItem->worldTransform_.data_.translate_, MakeRotateMatrix(data_.renderItem->worldTransform_.GetPWorldEulerTransformData()->rotate_));
	infos_[5].renderItem->worldTransform_.data_.translate_ += data_.renderItem->worldTransform_.GetPWorldEulerTransformData()->translate_;
}

void Wall::Draw(){
	for (int i = 0; i < 6; i++) {
		DrawManager::GetInstance()->PushBackOpaqueObject(&infos_[i]);
	}
}

void Wall::DontMove(){

}

void Wall::MoveAlternateLeftRight(){
	if (!isTurnAround_) {
		data_.renderItem->worldTransform_.data_.translate_.x += data_.moveSpeed * (1.0f / 60.0f);
		if (data_.renderItem->worldTransform_.data_.translate_.x >= keepData_.translate_.x + data_.moveLimit) {
			isTurnAround_ = true;
		}
	}
	else {
		data_.renderItem->worldTransform_.data_.translate_.x -= data_.moveSpeed * (1.0f / 60.0f);
		if (data_.renderItem->worldTransform_.data_.translate_.x <= keepData_.translate_.x - data_.moveLimit) {
			isTurnAround_ = false;
		}
	}
}

void Wall::MoveAlternateUpDown(){
	if (!isTurnAround_) {
		data_.renderItem->worldTransform_.data_.translate_.y += data_.moveSpeed * (1.0f / 60.0f);
		if (data_.renderItem->worldTransform_.data_.translate_.y >= keepData_.translate_.y + data_.moveLimit) {
			isTurnAround_ = true;
		}
	}
	else {
		data_.renderItem->worldTransform_.data_.translate_.y -= data_.moveSpeed * (1.0f / 60.0f);
		if (data_.renderItem->worldTransform_.data_.translate_.y <= keepData_.translate_.y - data_.moveLimit) {
			isTurnAround_ = false;
		}
	}
}

void Wall::MoveXRotate() {
	data_.renderItem->worldTransform_.data_.rotate_.x += (data_.rotateSpeed * (3.14f / 180.0f)) * (1.0f / 60.0f);
}

void Wall::MoveYRotate() {
	data_.renderItem->worldTransform_.data_.rotate_.y += (data_.rotateSpeed * (3.14f / 180.0f)) * (1.0f / 60.0f);
}

void Wall::MoveZRotate() {
	data_.renderItem->worldTransform_.data_.rotate_.z += (data_.rotateSpeed * (3.14f / 180.0f)) * (1.0f / 60.0f);
}