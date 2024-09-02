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

	isDebugCamera_ = false;

	//デバッグカメラ
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();

	cameraName_ = "inGame";

	//カメラが切り取る範囲
	nearClip_ = 0.1f;
	farClip_ = 100.0f;
	fovY_ = 0.45f;
}

void InGameCamera::Update() {
	InputManager* input = InputManager::GetInstance();

	//カメラの更新
#ifdef _DEBUG
	ImGui::Begin("Debug");
	ImGui::Checkbox("UseDebugCamera", &isDebugCamera_);
	ImGui::End();
#endif // _DEBUG

	Base3DCamera::Update();
	if (isDebugCamera_) {
		debugCamera_->Update();
		MainCamera::GetInstance()->Update(debugCamera_->GetWorldTransrom(), debugCamera_->GetWorldMatrix(), debugCamera_->GetProjectionMatrix());
	}
	else {
		transform_.translate_.z += 5.0f * (1.0f / 60.0f);
		MainCamera::GetInstance()->Update(transform_, worldMatrix_, projectionMatrix_);
	}
	
}