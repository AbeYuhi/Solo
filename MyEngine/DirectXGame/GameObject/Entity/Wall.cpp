#include "Wall.h"

Wall::Wall(){}
Wall::~Wall(){}

void Wall::Initialize(std::shared_ptr<MyEngine::Model> model,
	MyEngine::RenderItem* renderItem,
	Collider* collider,
	WallInfo info){

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

	//画面より後ろに行ったら消えるようにする処理
	data_.renderItem->worldTransform_.UpdateWorld();
	if (MainCamera::GetInstance()->GetWorldPos().z >= data_.renderItem->worldTransform_.worldPos_.z + 1.0f) {
		data_.collider->isDelete_ = true;
	}
}

void Wall::Draw(){
	data_.model->Draw(*data_.renderItem);
}

void Wall::DontMove(){

}

void Wall::MoveAlternateLeftRight(){

}

void Wall::MoveAlternateUpDown(){

}

void Wall::MoveXRotate() {

}

void Wall::MoveYRotate() {

}

void Wall::MoveZRotate() {

}