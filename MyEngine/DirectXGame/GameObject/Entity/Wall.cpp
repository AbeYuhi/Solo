#include "Wall.h"

Wall::Wall(){}
Wall::~Wall(){}

void Wall::Initialize(std::shared_ptr<MyEngine::Model> model,
	MyEngine::RenderItem* renderItem,
	Collider* collider,
	WallInfo info) {

	data_.model = model;
	data_.renderItem = renderItem;
	data_.collider = collider;
	data_.moveLimit = info.moveLimit;
	data_.moveSpeed = info.moveSpeed;
	data_.rotateSpeed = info.rotateSpeed;
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
		renderItems_[i].Initialize();
		renderItems_[i].worldTransform_.data_ = *data_.renderItem->worldTransform_.GetPWorldEulerTransformData();
	}
	//手前の面
	renderItems_[0].worldTransform_.data_.rotate_.x -= 3.14f / 2.0f;
	renderItems_[0].worldTransform_.data_.translate_.z -= data_.renderItem->worldTransform_.data_.scale_.z;
	renderItems_[0].worldTransform_.data_.scale_.z = data_.renderItem->worldTransform_.data_.scale_.y;
	renderItems_[0].materialInfo_.uvTransform_.scale_.x = scale.x;
	renderItems_[0].materialInfo_.uvTransform_.scale_.y = scale.y;
	//奥の面
	renderItems_[1].worldTransform_.data_.rotate_.x += 3.14f / 2.0f;
	renderItems_[1].worldTransform_.data_.translate_.z += data_.renderItem->worldTransform_.data_.scale_.z;
	renderItems_[1].worldTransform_.data_.scale_.z = data_.renderItem->worldTransform_.data_.scale_.y;
	renderItems_[1].materialInfo_.uvTransform_.scale_.x = scale.x;
	renderItems_[1].materialInfo_.uvTransform_.scale_.y = scale.y;
	//左の面
	renderItems_[2].worldTransform_.data_.rotate_.z += 3.14f / 2.0f;
	renderItems_[2].worldTransform_.data_.translate_.x -= data_.renderItem->worldTransform_.data_.scale_.x;
	renderItems_[2].worldTransform_.data_.scale_.x = data_.renderItem->worldTransform_.data_.scale_.y;
	renderItems_[2].materialInfo_.uvTransform_.scale_.x = scale.y;
	renderItems_[2].materialInfo_.uvTransform_.scale_.y = scale.z;
	//右の面
	renderItems_[3].worldTransform_.data_.rotate_.z -= 3.14f / 2.0f;
	renderItems_[3].worldTransform_.data_.translate_.x += data_.renderItem->worldTransform_.data_.scale_.x;
	renderItems_[3].worldTransform_.data_.scale_.x = data_.renderItem->worldTransform_.data_.scale_.y;
	renderItems_[3].materialInfo_.uvTransform_.scale_.x = scale.y;
	renderItems_[3].materialInfo_.uvTransform_.scale_.y = scale.z;
	//下の面
	renderItems_[4].worldTransform_.data_.rotate_.z += 3.14f;
	renderItems_[4].worldTransform_.data_.translate_.y -= data_.renderItem->worldTransform_.data_.scale_.y;
	renderItems_[4].materialInfo_.uvTransform_.scale_.x = scale.x;
	renderItems_[4].materialInfo_.uvTransform_.scale_.y = scale.z;
	//上の面
	renderItems_[5].worldTransform_.data_.translate_.y += data_.renderItem->worldTransform_.data_.scale_.y;
	renderItems_[5].materialInfo_.uvTransform_.scale_.x = scale.x;
	renderItems_[5].materialInfo_.uvTransform_.scale_.y = scale.z;
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
	Vector3 scale = { 0.0f, 0.0f, 0.0f };
	if (data_.renderItem->worldTransform_.parent_) {
		scale = { 
			std::ceil(data_.renderItem->worldTransform_.data_.scale_.x * data_.renderItem->worldTransform_.parent_->data_.scale_.x),
			std::ceil(data_.renderItem->worldTransform_.data_.scale_.y * data_.renderItem->worldTransform_.parent_->data_.scale_.y), 
			std::ceil(data_.renderItem->worldTransform_.data_.scale_.z * data_.renderItem->worldTransform_.parent_->data_.scale_.z) };
	}
	else {
		scale = { std::ceil(data_.renderItem->worldTransform_.data_.scale_.x), std::ceil(data_.renderItem->worldTransform_.data_.scale_.y), std::ceil(data_.renderItem->worldTransform_.data_.scale_.z) };
	}
	//ワールド情報の更新
	data_.renderItem->worldTransform_.UpdateWorld();
	//6面の移動
	for (int i = 0; i < 6; i++) {
		renderItems_[i].worldTransform_.data_ = *data_.renderItem->worldTransform_.GetPWorldEulerTransformData();
	}
	//手前の面
	renderItems_[0].worldTransform_.data_.rotate_.x -= 3.14f / 2.0f;
	renderItems_[0].worldTransform_.data_.scale_.z = data_.renderItem->worldTransform_.data_.scale_.y;
	renderItems_[0].materialInfo_.uvTransform_.scale_.x = scale.x;
	renderItems_[0].materialInfo_.uvTransform_.scale_.y = scale.y;
	if (data_.renderItem->worldTransform_.parent_) {
		renderItems_[0].worldTransform_.data_.translate_.z -= data_.renderItem->worldTransform_.data_.scale_.z * data_.renderItem->worldTransform_.parent_->data_.scale_.z;
		renderItems_[0].worldTransform_.data_.scale_.x *= data_.renderItem->worldTransform_.parent_->data_.scale_.x;
		renderItems_[0].worldTransform_.data_.scale_.z *= data_.renderItem->worldTransform_.parent_->data_.scale_.y;
	}
	else {
		renderItems_[0].worldTransform_.data_.translate_.z -= data_.renderItem->worldTransform_.data_.scale_.z;
	}
	//奥の面
	renderItems_[1].worldTransform_.data_.rotate_.x += 3.14f / 2.0f;
	renderItems_[1].worldTransform_.data_.scale_.z = data_.renderItem->worldTransform_.data_.scale_.y;
	renderItems_[1].materialInfo_.uvTransform_.scale_.x = scale.x;
	renderItems_[1].materialInfo_.uvTransform_.scale_.y = scale.y;
	if (data_.renderItem->worldTransform_.parent_) {
		renderItems_[1].worldTransform_.data_.translate_.z += data_.renderItem->worldTransform_.data_.scale_.z * data_.renderItem->worldTransform_.parent_->data_.scale_.z;
		renderItems_[1].worldTransform_.data_.scale_.x *= data_.renderItem->worldTransform_.parent_->data_.scale_.x;
		renderItems_[1].worldTransform_.data_.scale_.z *= data_.renderItem->worldTransform_.parent_->data_.scale_.y;
	}
	else {
		renderItems_[1].worldTransform_.data_.translate_.z += data_.renderItem->worldTransform_.data_.scale_.z;
	}
	//左の面
	renderItems_[2].worldTransform_.data_.rotate_.z += 3.14f / 2.0f;
	renderItems_[2].worldTransform_.data_.scale_.x = data_.renderItem->worldTransform_.data_.scale_.y;
	renderItems_[2].materialInfo_.uvTransform_.scale_.x = scale.y;
	renderItems_[2].materialInfo_.uvTransform_.scale_.y = scale.z;
	if (data_.renderItem->worldTransform_.parent_) {
		renderItems_[2].worldTransform_.data_.translate_.x -= data_.renderItem->worldTransform_.data_.scale_.x * data_.renderItem->worldTransform_.parent_->data_.scale_.x;
		renderItems_[2].worldTransform_.data_.scale_.x *= data_.renderItem->worldTransform_.parent_->data_.scale_.y;
		renderItems_[2].worldTransform_.data_.scale_.z *= data_.renderItem->worldTransform_.parent_->data_.scale_.z;
	}
	else {
		renderItems_[2].worldTransform_.data_.translate_.x -= data_.renderItem->worldTransform_.data_.scale_.x;
	}
	//右の面
	renderItems_[3].worldTransform_.data_.rotate_.z -= 3.14f / 2.0f;
	renderItems_[3].worldTransform_.data_.scale_.x = data_.renderItem->worldTransform_.data_.scale_.y;
	renderItems_[3].materialInfo_.uvTransform_.scale_.x = scale.y;
	renderItems_[3].materialInfo_.uvTransform_.scale_.y = scale.z;
	if (data_.renderItem->worldTransform_.parent_) {
		renderItems_[3].worldTransform_.data_.translate_.x += data_.renderItem->worldTransform_.data_.scale_.x * data_.renderItem->worldTransform_.parent_->data_.scale_.x;
		renderItems_[3].worldTransform_.data_.scale_.x *= data_.renderItem->worldTransform_.parent_->data_.scale_.y;
		renderItems_[3].worldTransform_.data_.scale_.z *= data_.renderItem->worldTransform_.parent_->data_.scale_.z;
	}
	else {
		renderItems_[3].worldTransform_.data_.translate_.x += data_.renderItem->worldTransform_.data_.scale_.x;
	}
	//下の面
	renderItems_[4].worldTransform_.data_.rotate_.z += 3.14f;
	renderItems_[4].materialInfo_.uvTransform_.scale_.x = scale.x;
	renderItems_[4].materialInfo_.uvTransform_.scale_.y = scale.z;
	if (data_.renderItem->worldTransform_.parent_) {
		renderItems_[4].worldTransform_.data_.translate_.y -= data_.renderItem->worldTransform_.data_.scale_.y * data_.renderItem->worldTransform_.parent_->data_.scale_.y;
		renderItems_[4].worldTransform_.data_.scale_.x *= data_.renderItem->worldTransform_.parent_->data_.scale_.x;
		renderItems_[4].worldTransform_.data_.scale_.z *= data_.renderItem->worldTransform_.parent_->data_.scale_.z;
	}
	else {
		renderItems_[4].worldTransform_.data_.translate_.y -= data_.renderItem->worldTransform_.data_.scale_.y;
	}
	//上の面
	renderItems_[5].materialInfo_.uvTransform_.scale_.x = scale.x;
	renderItems_[5].materialInfo_.uvTransform_.scale_.y = scale.z;
	if (data_.renderItem->worldTransform_.parent_) {
		renderItems_[5].worldTransform_.data_.translate_.y += data_.renderItem->worldTransform_.data_.scale_.y * data_.renderItem->worldTransform_.parent_->data_.scale_.y;
		renderItems_[5].worldTransform_.data_.scale_.x *= data_.renderItem->worldTransform_.parent_->data_.scale_.x;
		renderItems_[5].worldTransform_.data_.scale_.z *= data_.renderItem->worldTransform_.parent_->data_.scale_.z;
	}
	else {
		renderItems_[5].worldTransform_.data_.translate_.y += data_.renderItem->worldTransform_.data_.scale_.y;
	}
}

void Wall::Draw(){
	for (int i = 0; i < 6; i++) {
		data_.model->Draw(renderItems_[i]);
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