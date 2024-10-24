#pragma once
#include "DirectXGame/Data/Transform.h"
#include "DirectXGame/Math/Vector2.h"
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Math/Matrix4x4.h"
#include "DirectXGame/Math/Math.h"
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/GameObject/Camera/Base3DCamera.h"
#include "DirectXGame/Manager/ImGuiManager.h"
#include "DirectXGame/Manager/InputManager.h"
#include "GameObject/Camera/MainCamera.h"
#include "GameObject/Camera/DebugCamera.h"

/// <summary>
/// InGameCamera.h
/// インゲームシーンのカメラを処理しているファイル
/// </summary>

class InGameCamera : public Base3DCamera
{
public:
	InGameCamera();
	~InGameCamera();

	void Initialize() override;

	void Update() override;

private:
	//デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_;
	bool isDebugCamera_;
};
