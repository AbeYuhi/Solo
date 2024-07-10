#include "Door.h"

Door::Door(){}
Door::~Door(){}

void Door::Initialize() {

	model_ = Model::Create("cube", "cube.obj");
	leftDoorInfo_.Initialize();
	rightDoorInfo_.Initialize();

}

void Door::Update() {

}

void Door::Draw() {

}