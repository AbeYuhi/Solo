#pragma once
#include "DirectXGame/Data/Transform.h"
#include "DirectXGame/Math/Vector2.h"
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Math/Matrix4x4.h"
#include "DirectXGame/Math/Math.h"
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Manager/ImGuiManager.h"
#include "DirectXGame/Manager/InputManager.h"
#include "Manager/CollisionManager.h"

/// <summary>
/// Base3DCamera.h
/// カメラの規定クラス
/// </summary>

class Base3DCamera
{
public:
	Base3DCamera();
	~Base3DCamera();

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// オブジェクトデータの設定処理
	/// </summary>
	/// <param name="data"></param>
	inline void SetWorldTransrom(EulerTransformData data) { transform_ = data; }

	/// <summary>
	/// ワールド座標の指定処理
	/// </summary>
	/// <param name="pos"></param>
	inline void SetPos(Vector3 pos) { transform_.translate_ = pos; }

	/// <summary>
	/// 親とペアレントする処理
	/// </summary>
	/// <param name="data"></param>
	inline void ParentWorldTransrom(const EulerTransformData& data) { transform_ = data; }

	/// <summary>
	/// ワールドデータを取得する処理
	/// </summary>
	/// <returns></returns>
	inline EulerTransformData GetWorldTransrom() { return transform_; }

	/// <summary>
	/// ワールドデータのポインタを取得する処理
	/// </summary>
	/// <returns></returns>
	inline EulerTransformData* GetPWorldTransrom() { return &transform_; }

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
	/// ニアクリップの取得処理
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

public:
	EulerTransformData transform_;

protected:
	Matrix4x4 worldMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewProjectionMatrix_;
	float nearClip_;
	float farClip_;
	float fovY_;

	std::string cameraName_;
};

