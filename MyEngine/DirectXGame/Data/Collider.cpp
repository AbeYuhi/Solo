#include "Collider.h"

void Collider::Initialize(Vector3* translate, Vector3 colliderScale, ColliderTag tag, CollisionType type, bool isCollisionCheck, Vector3* velocity, bool isDrawCollider) {
	translate_ = translate;
	colliderScale_ = colliderScale;
	tag_ = tag;
	type_ = type;
	velocity_ = velocity;
	isDrawCollider_ = isDrawCollider;
	isCollisionCheck_ = isCollisionCheck;

	//デバック時にコライダーの衝突範囲を可視化するためにrenderItemを初期化する
#ifdef _DEBUG
	renderItem_.Initialize();
#endif // _DEBUG
	/*for (int i = 0; i < kNumColliderTag; i++) {
		switch (type_)
		{
		case BOX:
			collision_[i] = BoxCollision();
			break;
		case SPHERE:
			collision_[i] = SphereCollision();
			break;
		default:
			break;
		}
	}*/
	isDelete_ = false;
	isPush_ = false;
}

void Collider::Update() {

	aabb_.min.x = translate_->x - (colliderScale_.x / 2.0f);
	aabb_.min.y = translate_->y - (colliderScale_.y / 2.0f);
	aabb_.min.z = translate_->z - (colliderScale_.z / 2.0f);

	aabb_.max.x = translate_->x + (colliderScale_.x / 2.0f);
	aabb_.max.y = translate_->y + (colliderScale_.y / 2.0f);
	aabb_.max.z = translate_->z + (colliderScale_.z / 2.0f);

	ControlMinMax(aabb_);
}
