#include "PlayerBullet.h"

/// <summary>
/// PlayerBullet.cpp
/// プレイヤー弾クラスの実装ファイル
/// </summary>

void PlayerBullet::Initialize(Vector2 mousePos) {
	input_ = MyEngine::InputManager::GetInstance();

	uint32_t environmentTextureHandle = MyEngine::TextureManager::Load("rostock_laage_airport_4k.dds");

	model_ = MyEngine::Model::Create("sphere", "sphere.obj");
	renderItem_.Initialize();
	renderItem_.worldTransform_.data_.scale_ *= 0.5f;
	renderItem_.materialInfo_.material_->enableLightint = true;
	renderItem_.materialInfo_.material_->isEnvironment = true;
	renderItem_.materialInfo_.environmentTextureHandle_ = environmentTextureHandle;

	//発射位置の計算
	renderItem_.worldTransform_.data_.translate_ = MainCamera::GetInstance()->GetWorldPos();
	Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, MyEngine::WinApp::kWindowWidth, MyEngine::WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matVPV = MainCamera::GetInstance()->GetViewProjectionMatrix() * viewportMatrix;
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	Vector2 spritePosition = mousePos;
	spritePosition.y -= 100;

	//スクリーン座標
	Vector3 posNear = Vector3((float)spritePosition.x, (float)spritePosition.y, 0);
	Vector3 posFar = Vector3((float)spritePosition.x, (float)spritePosition.y, 1);
	//スクリーン座標系からワールド座標系へ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	//マウスレイの方向
	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = Normalize(mouseDirection);
	//カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 20;
	reticlePos_ = posNear + (mouseDirection * kDistanceTestObject);

	velocity_ = Normalize(reticlePos_ - MainCamera::GetInstance()->GetWorldPos());
	velocity_.z *= 1.25f;
	speed_ = 40.0f;

	lifeTime_ = 15.0f;

	collider_.Initialize(&renderItem_.worldTransform_, {.scale_ = {2, 2, 2}, .rotate_ = {0, 0, 0}, .translate_ = {0, 0, 0}}, ColliderTag::BULLET, kSPHERE, true, &velocity_);
	MyEngine::CollisionManager::GetInstance()->AddCollider(&collider_);

	isGround_ = false;
}

void PlayerBullet::Update() {
	lifeTime_ -= 1.0f / 60.0f;

	if (collider_.isContact_[WALL] || collider_.isContact_[BUTTON] || collider_.isContact_[LDOOR] || collider_.isContact_[RDOOR]) {
		collider_.reflection_ = CalculateReflection(*collider_.velocity_, collider_.normal_);
		collider_.objData_->data_.translate_ = collider_.contactPoint_ + (collider_.normal_ * (collider_.combinedScale / 2.0f));
		velocity_ = collider_.reflection_;

		// 法線ベクトル（衝突面の法線）
		Vector3 normal = collider_.normal_;

		// 速度を法線方向と接線方向に分解
		Vector3 velocityNormal = Dot(velocity_, normal) * normal; // 法線方向の速度成分
		Vector3 velocityTangent = velocity_ - velocityNormal;     // 接線方向の速度成分

		// 減衰率を設定
		float normalDamping = 0.5f;  // 法線方向（正面衝突）の減衰率
		float tangentDamping = 0.7f; // 接線方向（横滑り）の減衰率

		// 法線方向の速度が逆向きにならないように調整
		if (Dot(velocityNormal, normal) < 0.0f) {
			velocityNormal = Vector3(0.0f, 0.0f, 0.0f); // 法線方向の速度をゼロに
		}
		else {
			velocityNormal *= normalDamping;
		}

		// 速度の減衰を適用
		velocityTangent *= tangentDamping;

		// 新しい速度を計算
		velocity_ = velocityTangent + velocityNormal;

		if (collider_.normal_.y == 1.0f && velocity_.y < 0.05f) {
			velocity_.y = 0.0f;
			isGround_ = true;
		}
	}
	if (collider_.isContact_[GLASS]) {
		collider_.reflection_ = CalculateReflection(*collider_.velocity_, collider_.normal_);
		velocity_ += Normalize(collider_.reflection_) * 0.4f;
		velocity_ *= 0.5f;
	}

	//重力の加算
	if (!isGround_) {
		const float gravity = 9.8f * (1.0f / 60.0f);
		velocity_.y -= gravity * (1.0f / 60.0f);
	}

	// 摩擦による減速
	if (isGround_) {
		const float friction = 0.98f;
		velocity_.x *= friction * (1.0f / 60.0f);
		velocity_.z *= friction * (1.0f / 60.0f);
		velocity_.y = 0.0f;

		if (std::abs(velocity_.x) < 0.01f && std::abs(velocity_.z) < 0.01f) {
			velocity_.x = 0.0f;
			velocity_.z = 0.0f;
		}
	}

	// 位置の更新
	renderItem_.worldTransform_.data_.translate_ += velocity_ * speed_ * (1.0f / 60.0f);
}

void PlayerBullet::Draw() {

	model_->Draw(renderItem_);

}

void PlayerBullet::ParticleDraw() {

}