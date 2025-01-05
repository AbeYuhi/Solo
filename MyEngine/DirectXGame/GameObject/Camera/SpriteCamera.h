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
/// SpriteCamera.h
/// スプライﾄのカメラを処理しているファイル
/// </summary>

class SpriteCamera
{
public:
	/// <summary>
	/// インスタンスの取得処理
	/// </summary>
	/// <returns></returns>
	static SpriteCamera* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// カメラ情報の処理
	/// </summary>
	/// <returns></returns>
	inline EulerTransformData GetWorldTransrom() { return transform_; }

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
	inline const Matrix4x4* GetPViewProjectionMatrix() { return &viewProjectionMatrix_; }

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

private:
	SpriteCamera() = default;
	~SpriteCamera() = default;

	EulerTransformData transform_;
	Matrix4x4 worldMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewProjectionMatrix_;
	float nearClip_;
	float farClip_;
	bool isMove_;
};