#include "TitleCamera.h"

/// <summary>
/// MenuCamera.cpp
/// タイトルシーンカメラクラスの実装ファイル
/// </summary>

TitleCamera::TitleCamera(){}
TitleCamera::~TitleCamera(){}

void TitleCamera::Initialize() {
	
	//カメラの初期化
	Base3DCamera::Initialize();

	cameraName_ = "Title";

	//カメラが切り取る範囲
	nearClip_ = 0.1f;
	farClip_ = 1000.0f;
	fovY_ = 0.45f;
}

void TitleCamera::Update() {



	Base3DCamera::Update();
}
