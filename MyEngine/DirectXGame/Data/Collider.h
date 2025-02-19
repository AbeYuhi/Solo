#pragma once
#include <variant>
#include "DirectXGame/Math/AABB.h"
#include "DirectXGame/Math/OBB.h"
#include "DirectXGame/Math/Sphere.h"
#include "DirectXGame/Math/Vector2.h"
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Math/Matrix3x3.h"
#include "DirectXGame/Data/Transform.h"
#include "DirectXGame/Data/RenderItem.h"
#include "Manager/ImGuiManager.h"

/// <summary>
/// Collider.h
/// コライダーの情報をまとめているファイル
/// </summary>

enum ColliderTag {
	PLAYER,
	BULLET,
	GOAL,
	WALL,
	CRYSTAL,
	GLASS,
	BUTTON,
	LDOOR,
	RDOOR,
	CAMERA,
	kNumColliderTag,
};

enum ColliderType {
	kAABB,
	kOBB,
	kSPHERE
};

struct Collider {
	MyEngine::WorldTransform* objData_;
	EulerTransformData colliderData_;
	Matrix4x4 worldMatrix_;
	Vector3 scale_;
	Vector3* velocity_;
	Vector3 contactPoint_;
	Vector3 normal_;
	Vector3 reflection_;
	ColliderTag tag_;
	ColliderType type_;
	std::variant<AABB, OBB, Sphere> colliderShape_;

	//合成された位置
	Vector3 combinedPosition;
	// 合成された回転
	Vector3 combinedRotation;
	// 合成された大きさ
	Vector3 combinedScale;

#ifdef _DEBUG
	MyEngine::RenderItem renderItem_;
#endif // _DEBUG

	bool isContact_[kNumColliderTag];

	//押しつぶされているか
	bool isPush_;

	//Colliderを消すか
	bool isDelete_;

	//衝突判定を描画するか
	bool isDrawCollider_;

	bool isCollisionCheck_;
	
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="objData"></param>
	/// <param name="colliderData"></param>
	/// <param name="tag"></param>
	/// <param name="type"></param>
	/// <param name="isCollisionCheck"></param>
	/// <param name="velocity"></param>
	/// <param name="isDrawCollider"></param>
	void Initialize(MyEngine::WorldTransform* objData, EulerTransformData colliderData, ColliderTag tag, ColliderType type, bool isCollisionCheck, Vector3* velocity = nullptr, bool isDrawCollider = true);
	
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
};