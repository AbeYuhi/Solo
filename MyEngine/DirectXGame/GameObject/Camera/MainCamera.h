#pragma once
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Base/CreateResource/CreateResource.h"
#include "DirectXGame/Manager/GraphicsPipelineManager.h"
#include "DirectXGame/Math/Vector2.h"
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Math/Vector4.h"
#include "DirectXGame/Math/Matrix4x4.h"
#include "DirectXGame/Math/Math.h"
#include "DirectXGame/Data/CameraForGPU.h"
#include "DirectXGame/Data/Transform.h"

/// <summary>
/// MainCamera.h
/// シェーダーに送るためにカメラを管理しているファイル
/// </summary>

class MainCamera {
public:
	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static MainCamera* GetInstance();

	/// <summary>
	/// カメラのリソースなどの初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// カメラの更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// カメラの更新処理
	/// </summary>
	void Update(EulerTransformData transform_, Matrix4x4 worldMatrix = MakeIdentity4x4(), Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, (float)MyEngine::WinApp::kWindowWidth / (float)MyEngine::WinApp::kWindowHeight, 0.1f, 100.0f));

	/// <summary>
	/// カメラの情報をGPUに送るための処理
	/// </summary>
	void Draw();

	/// <summary>
	/// ワールドマトリックスの取得処理
	/// </summary>
	/// <returns></returns>
	Matrix4x4 GetWorldMatrix() const { return worldMatrix_; }

	/// <summary>
	/// ビュープロジェクションの取得処理
	/// </summary>
	/// <returns></returns>
	Matrix4x4 GetViewProjectionMatrix() const { return viewProjectionMatrix_; }

	/// <summary>
	/// ビュープロジェクションのポインタ取得処理
	/// </summary>
	/// <returns></returns>
	Matrix4x4* GetPViewProjectionMatrix() { return &viewProjectionMatrix_; }

	/// <summary>
	/// ビューマトリックスの取得処理
	/// </summary>
	/// <returns></returns>
	Matrix4x4 GetViewMatrix() const { return viewMatrix_; }

	/// <summary>
	/// プロジェクションマトリックスの取得処理
	/// </summary>
	/// <returns></returns>
	Matrix4x4 GetProjectionMatrix() const { return projectionMatrix_; }

	/// <summary>
	/// ワールド座標のカメラ位置の取得処理
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPos() const { return { worldMatrix_.m[3][0], worldMatrix_.m[3][1] , worldMatrix_.m[3][2] }; }

public:
	EulerTransformData transform_;

private:
	MainCamera() = default;
	~MainCamera() = default;
	ComPtr<ID3D12Resource> cameraResorce_ = nullptr;
	CameraForGPU* cameraData_;
	Matrix4x4 worldMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewProjectionMatrix_;
};