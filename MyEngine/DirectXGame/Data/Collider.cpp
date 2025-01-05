#include "Collider.h"

/// <summary>
/// Collider.cpp
/// コライダーに関する関数の実装ファイル
/// </summary>

void Collider::Initialize(MyEngine::WorldTransform* objData, EulerTransformData colliderData, ColliderTag tag, ColliderType type, bool isCollisionCheck, Vector3* velocity, bool isDrawCollider) {
	objData_ = objData;
	colliderData_ = colliderData;
	worldMatrix_ = MakeAffineMatrix(colliderData_);
	worldMatrix_ = Multiply(worldMatrix_, MakeAffineMatrix(*objData_->GetPWorldEulerTransformData()));
	tag_ = tag;
	type_ = type;
	velocity_ = velocity;
	isDrawCollider_ = isDrawCollider;
	isCollisionCheck_ = isCollisionCheck;

	//デバック時にコライダーの衝突範囲を可視化するためにrenderItemを初期化する
#ifdef _DEBUG
	renderItem_.Initialize();
#endif // _DEBUG
	for (int i = 0; i < kNumColliderTag; i++) {
		isContact_[i] = false;
		normal_ = Vector3{ 0.0f, 0.0f, 0.0f };
		contactPoint_ = Vector3{ 0.0f, 0.0f, 0.0f };
	}
	isDelete_ = false;
	isPush_ = false;

	switch (type_)
	{
	case kAABB:
		colliderShape_ = AABB();
		break;
	case kOBB:
		colliderShape_ = OBB();
		break;
	case kSPHERE:
		colliderShape_ = Sphere();
		break;
	default:
		break;
	}
}

void Collider::Update() {
	objData_->UpdateWorld();
	Matrix4x4 objMatrix = MakeAffineMatrix(*objData_->GetPWorldEulerTransformData());
	Matrix4x4 colliderMatrix = MakeAffineMatrix(colliderData_);
	Matrix4x4 combinedMatrix = Multiply(colliderMatrix, objMatrix);

	EulerTransformData combinedData = ExtractTransform(combinedMatrix);

	//// 合成された位置
	//combinedPosition = combinedData.translate_;
	//// 合成された回転
	//combinedRotation = combinedData.rotate_;
	//// 合成されたスケール
	//combinedScale = combinedData.scale_;

	// 合成された位置
	combinedPosition = objData_->GetPWorldEulerTransformData()->translate_;
	// 合成された回転
	combinedRotation = objData_->GetPWorldEulerTransformData()->rotate_;
	// 合成されたスケール
	if (objData_->parent_) {
		std::vector<Vector3> scales;
		MyEngine::WorldTransform worldTransform;
		worldTransform = *objData_;
		while (true) {
			if (worldTransform.parent_) {
				Vector3 parentScale;
				parentScale = worldTransform.parent_->data_.scale_;
				scales.push_back(parentScale);
				worldTransform = *worldTransform.parent_;
			}
			else {
				break;
			}
		}
		combinedScale = objData_->GetPWorldEulerTransformData()->scale_ * colliderData_.scale_;
		for (int i = 0; i < scales.size(); i++) {
			combinedScale *= scales[i];
		}
	}
	else {
		combinedScale = objData_->GetPWorldEulerTransformData()->scale_ * colliderData_.scale_;
	}
	
	//形状に合わせた処理
	std::visit([&](auto& shape) {
		using T = std::decay_t<decltype(shape)>;
		if constexpr (std::is_same_v<T, AABB>) {
			shape.min.x = combinedPosition.x - (combinedScale.x / 2.0f);
			shape.min.y = combinedPosition.y - (combinedScale.y / 2.0f);
			shape.min.z = combinedPosition.z - (combinedScale.z / 2.0f);
			shape.max.x = combinedPosition.x + (combinedScale.x / 2.0f);
			shape.max.y = combinedPosition.y + (combinedScale.y / 2.0f);
			shape.max.z = combinedPosition.z + (combinedScale.z / 2.0f);
			ControlMinMax(shape);
		}
		else if constexpr (std::is_same_v<T, OBB>) {
			shape.center = combinedPosition;
			Matrix4x4 rotateMatrix = MakeRotateMatrix(combinedRotation);
			shape.orientations[0].x = rotateMatrix.m[0][0];
			shape.orientations[0].y = rotateMatrix.m[0][1];
			shape.orientations[0].z = rotateMatrix.m[0][2];

			shape.orientations[1].x = rotateMatrix.m[1][0];
			shape.orientations[1].y = rotateMatrix.m[1][1];
			shape.orientations[1].z = rotateMatrix.m[1][2];

			shape.orientations[2].x = rotateMatrix.m[2][0];
			shape.orientations[2].y = rotateMatrix.m[2][1];
			shape.orientations[2].z = rotateMatrix.m[2][2];
			shape.size = combinedScale;
		}
		else if constexpr (std::is_same_v<T, Sphere>) {
			shape.center = combinedPosition;
			shape.radius = combinedScale.x / 2.0f;
		}
	}, colliderShape_);
}
