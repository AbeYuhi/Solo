#include "MainCamera.h"

/// <summary>
/// MainCamera.cpp
/// GPUに3D用カメラの情報を送信するための情報をまとめられているクラスの実装ファイル
/// </summary>

MainCamera* MainCamera::GetInstance() {
	static MainCamera instance;
	return &instance;
}

void MainCamera::Initialize() {
	transform_.Initialize();

	viewProjectionMatrix_ = MakeIdentity4x4();

	//Resourceの生成
	cameraResorce_ = MyEngine::CreateBufferResource(sizeof(CameraForGPU));

	//Resourceにデータを書き込む
	cameraResorce_->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));
	cameraData_->worldPosition = {0};
}

void MainCamera::Update() {
	worldMatrix_ = MakeAffineMatrix(transform_);
	viewMatrix_ = Inverse(worldMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, (float)MyEngine::WinApp::kWindowWidth / (float)MyEngine::WinApp::kWindowHeight, 0.1f, 1000.0f);
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
}

void MainCamera::Update(EulerTransformData transform, Matrix4x4 worldMatrix, Matrix4x4 projectionMatrix) {
	transform_ = transform;
	worldMatrix_ = MakeAffineMatrix(transform);
	viewMatrix_ = Inverse(worldMatrix_);
	projectionMatrix_ = projectionMatrix;
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
	cameraData_->worldPosition = { worldMatrix.m[3][0], worldMatrix.m[3][1], worldMatrix.m[3][2] };
}

void MainCamera::Draw() {
	MyEngine::DirectXCommon* dxCommon = MyEngine::DirectXCommon::GetInstance();
	//パイプラインステートの設定
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(4, cameraResorce_->GetGPUVirtualAddress());
}