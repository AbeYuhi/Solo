#include "CollisionManager.h"

/// <summary>
/// CollisionManager.h
/// 衝突判定を管理しているクラスの実装ファイル
/// </summary>

namespace MyEngine {

	CollisionManager* CollisionManager::GetInstance() {
		static CollisionManager instance;
		return &instance;
	}

	void CollisionManager::Initialize() {
		colliders_.clear();

		wireFrameBox_ = WireFrameBox::Create();
		wireFrameSphere_ = WireFrameSphere::Create();
	}

	void CollisionManager::Update() {

		//コライダーを破壊
		SyncColliderList();

		//コライダーの更新
		for (auto& collider : colliders_) {
			collider->Update();
		}

		//衝突判定
		for (auto& collider0 : colliders_) {
			if (collider0->isCollisionCheck_) {
				for (int i = 0; i < kNumColliderTag; i++) {
					collider0->isContact_[i] = false;
					collider0->normal_ = {0, 0, 0};
				}
				for (auto& collider1 : colliders_) {

					if (collider0 != collider1) {
						std::visit([&](auto& shape0) {
							std::visit([&](auto& shape1) {
								if (IsCollision(shape0, shape1)) {
									collider0->isContact_[collider1->tag_] = true;
									if (collider0->velocity_) {
										Vector3 tmpNormal = Vector3{ 0.0f, 0.0f, 0.0f };
										Vector3 tmpContactPoint = Vector3{ 0.0f, 0.0f, 0.0f };
										if (collider0->normal_ != Vector3{ 0.0f, 0.0f, 0.0f }) {
											tmpNormal = collider0->normal_;
											tmpContactPoint = collider0->contactPoint_;
											collider0->normal_ = CalculateNormal(shape0, shape1);
											//接触点の計算
											if (Length(GetClosestPointOnOBB(shape0, *collider0->velocity_, shape1) - collider0->combinedPosition) <= Length(tmpContactPoint - collider0->combinedPosition)) {
												collider0->contactPoint_ = GetClosestPointOnOBB(shape0, *collider0->velocity_, shape1);
											}
											else {
												collider0->normal_ = tmpNormal;
											}
										}
										else {
											collider0->normal_ = CalculateNormal(shape0, shape1);
											collider0->contactPoint_ = GetClosestPointOnOBB(shape0, *collider0->velocity_, shape1);
										}
									}
								}
							}, collider1->colliderShape_);
						}, collider0->colliderShape_);
					}

				}
			}
		}
	}

	void CollisionManager::Draw() {
#ifdef _DEBUG
		for (auto& collider : colliders_) {

			if (collider->isDrawCollider_) {
				collider->renderItem_.worldTransform_.data_.translate_ = collider->combinedPosition;
				collider->renderItem_.worldTransform_.data_.rotate_ = collider->combinedRotation;
				collider->renderItem_.worldTransform_.data_.scale_ = collider->combinedScale;
				if (collider->type_ == kSPHERE) {
					wireFrameSphere_->Draw(collider->renderItem_);
				}
				else {
					wireFrameBox_->Draw(collider->renderItem_);
				}
			}
		}
#endif // _DEBUG
	}

	void CollisionManager::SyncColliderList() {
		for (auto it = colliders_.begin(); it != colliders_.end();) {
			Collider* collider = *it;
			if (collider->isDelete_) {
				// 要素を削除
				it = colliders_.erase(it);	
			}
			else {
				++it;
			}
		}
	}

	void CollisionManager::Reset() {
		colliders_.clear();
	}

}