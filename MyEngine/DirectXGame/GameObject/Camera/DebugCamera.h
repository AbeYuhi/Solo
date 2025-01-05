#pragma once
#include "DirectXGame/Data/Transform.h"
#include "DirectXGame/Math/Vector2.h"
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Math/Matrix4x4.h"
#include "DirectXGame/Math/Math.h"
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Manager/ImGuiManager.h"
#include "DirectXGame/Manager/InputManager.h"
#define M_PI 3.14f

/// <summary>
/// DebugCamera.h
/// デバッグカメラを処理しているファイル
/// </summary>

class DebugCamera
{
public:
	DebugCamera();
	~DebugCamera();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// カメラ情報の取得処理
	/// </summary>
	/// <returns></returns>
	inline EulerTransformData GetWorldTransrom() { return transform_; }

	/// <summary>
	/// ワールドマトリックスの取得処理
	/// </summary>
	/// <returns></returns>
	inline Matrix4x4 GetWorldMatrix() { return worldMatrix_; }

	/// <summary>
	/// ビューマトリックスの取得処理
	/// </summary>
	/// <returns></returns>
	inline Matrix4x4 GetViewMatrix() { return viewMatrix_; }

	/// <summary>
	/// プロジェクションマトリックスの取得処理
	/// </summary>
	/// <returns></returns>
	inline Matrix4x4 GetProjectionMatrix() { return projectionMatrix_; }

	/// <summary>
	/// ビュープロジェクションマトリックスの取得処理
	/// </summary>
	/// <returns></returns>
	inline Matrix4x4 GetViewProjectionMatrix() { return viewProjectionMatrix_; }

	/// <summary>
	/// ビュープロジェクションマトリックスのポインタ取得処理
	/// </summary>
	/// <returns></returns>
	inline const Matrix4x4* GetViewProjectionMatrixPointer() { return &viewProjectionMatrix_; }

	/// <summary>
	/// ニアークリップの取得処理
	/// </summary>
	/// <returns></returns>
	inline float GetNearClip() { return nearClip_; }

	/// <summary>
	/// ファークリップの取得処理
	/// </summary>
	/// <returns></returns>
	inline float GetFarClip() { return farClip_; }

	/// <summary>
	/// FovYの取得処理
	/// </summary>
	/// <returns></returns>
	inline float GetFovY() { return fovY_; }

	/// <summary>
	/// カメラを動かすかの取得処理
	/// </summary>
	/// <returns></returns>
	inline bool IsMove() { return isMove_; }

	/// <summary>
	/// カメラを動かせるようにするかをセットする処理
	/// </summary>
	/// <param name="isMove"></param>
	inline void SetMove(bool isMove) { isMove_ = isMove; }

	/// <summary>
	/// カメラの位置のセッター
	/// </summary>
	/// <param name="pos"></param>
	inline void SetPos(Vector3 pos) { transform_.translate_ = pos; }

	/// <summary>
	/// カメラの回転のセッター
	/// </summary>
	/// <param name="rotate"></param>
	inline void SetRotate(Vector3 rotate) { transform_.rotate_ = rotate; }

private:
	
	EulerTransformData transform_;
	Matrix4x4 worldMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewProjectionMatrix_;
	float nearClip_;
	float farClip_;
	float fovY_;

	bool isMove_;
};
