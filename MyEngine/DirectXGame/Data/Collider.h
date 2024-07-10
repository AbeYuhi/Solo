#pragma once
#include <variant>
#include "DirectXGame/Math/AABB.h"
#include "DirectXGame/Math/Vector2.h"
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Data/Transform.h"
#include "DirectXGame/Data/RenderItem.h"

enum ColliderTag {
	PLAYER,
	BUTTON,
	BULLET,
	WALL,
	kNumColliderTag,
};

enum CollisionType {
	BOX,
	SPHERE,
};

struct BoxCollision {

	BoxCollision() {
		isContact_ = false;
		isUnderHit_ = false;
		isTopHit_ = false;
		isLeftHit_ = false;
		isRightHit_ = false;
		isFrontHit_ = false;
		isBackHit_ = false;

		isTopLeftFrontHit_ = false;
		isTopRightFrontHit_ = false;
		isUnderLeftFrontHit_ = false;
		isUnderRightFrontHit_ = false;
		isTopLeftBackHit_ = false;
		isTopRightBackHit_ = false;
		isUnderLeftBackHit_ = false;
		isUnderRightBackHit_ = false;
	}

	//どこと衝突しているか
	bool isContact_;

	//6面
	bool isUnderHit_;
	bool isTopHit_;
	bool isLeftHit_;
	bool isRightHit_;
	bool isFrontHit_;
	bool isBackHit_;

	//8頂点
	bool isTopLeftFrontHit_;
	bool isTopRightFrontHit_;
	bool isUnderLeftFrontHit_;
	bool isUnderRightFrontHit_;
	bool isTopLeftBackHit_;
	bool isTopRightBackHit_;
	bool isUnderLeftBackHit_;
	bool isUnderRightBackHit_;
};

struct SphereCollision {
	//どこと衝突しているか
	bool isContact_;
};

using Collision = std::variant<BoxCollision, SphereCollision>;

struct Collider {
	AABB aabb_;
	Vector3* translate_;
	Vector3* velocity_;
	Vector3 colliderScale_;
	Vector3 contactPoint_;
	ColliderTag tag_;
	CollisionType type_;
	RenderItem renderItem_;

	Collision collision_[kNumColliderTag];

	//押しつぶされているか
	bool isPush_;

	//Colliderを消すか
	bool isDelete_;

	//衝突判定を描画するか
	bool isDrawCollider_;

	bool isCollisionCheck_;

	void Initialize(Vector3* translate, Vector3 colliderScale, ColliderTag tag, CollisionType type, bool isCollisionCheck, Vector3* velocity = nullptr, bool isDrawCollider = true);
	void Update();
};