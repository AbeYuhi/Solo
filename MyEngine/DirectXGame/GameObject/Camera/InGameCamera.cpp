#include "InGameCamera.h"

InGameCamera::InGameCamera()
{
}

InGameCamera::~InGameCamera()
{
}

void InGameCamera::Initialize() {

	//カメラの初期化
	Base3DCamera::Initialize();

	transform_.translate_.z = -120;
	transform_.translate_.y = 10;

	cameraName_ = "inGame";

	//カメラが切り取る範囲
	nearClip_ = 0.1f;
	farClip_ = 100.0f;
	fovY_ = 0.45f;
}

void InGameCamera::Update() {
	InputManager* input = InputManager::GetInstance();

	transform_.translate_.z += 10.0f * (1.0f / 60.0f);

	Base3DCamera::Update();
}