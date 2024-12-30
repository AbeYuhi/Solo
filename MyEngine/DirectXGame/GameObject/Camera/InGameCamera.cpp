#include "InGameCamera.h"

/// <summary>
/// InGameCamera.cpp
/// ゲームクリアシーンカメラクラスの実装ファイル
/// </summary>

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
	farClip_ = 1000.0f;
	fovY_ = 0.45f;
}

void InGameCamera::Update() {

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
		MainCamera::GetInstance()->Update(transform_, worldMatrix_, projectionMatrix_);
	}
	
}