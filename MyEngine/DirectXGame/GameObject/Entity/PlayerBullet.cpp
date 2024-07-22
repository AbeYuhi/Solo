#include "PlayerBullet.h"

PlayerBullet::PlayerBullet(){}
PlayerBullet::~PlayerBullet(){}

void PlayerBullet::Initialize() {
	input_ = InputManager::GetInstance();

	model_ = Model::Create("sphere", "sphere.obj");
	renderItem_.Initialize();
	renderItem_.materialInfo_.material_->color = { 1.0f, 0.0f, 0.0f, 1.0f };

	//発射位置の計算
	renderItem_.worldTransform_.data_.translate_ = MainCamera::GetInstance()->GetWorldPos();
	Vector2 mousePos = input_->GetMousePos();
	Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matVPV = MainCamera::GetInstance()->GetViewProjectionMatrix() * viewportMatrix;
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	Vector2 spritePosition = mousePos;

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
	velocity_ *= 10.0f;

	collider_.Initialize(&renderItem_.worldTransform_.data_.translate_, renderItem_.worldTransform_.data_.scale_, { 2, 2, 2 }, ColliderTag::BULLET, true, &velocity_);
	CollisionManager::GetInstance()->AddCollider(&collider_);
}

void PlayerBullet::Update() {
	renderItem_.worldTransform_.data_.translate_ += velocity_ * (1.0f / 60.0f);

	if (collider_.collision_[WALL].isContact_) {

		if (collider_.collision_[WALL].isUnderHit_) {
			velocity_.y = 10;
			velocity_.x = 0;
			velocity_.z = 0;
		}

		if (collider_.collision_[WALL].isLeftHit_) {
			velocity_.y = 0;
			velocity_.x = 10;
			velocity_.z = 0;
		}

		if (collider_.collision_[WALL].isRightHit_) {
			velocity_.y = 0;
			velocity_.x = -10;
			velocity_.z = 0;
		}

		if (collider_.collision_[WALL].isFrontHit_) {
			velocity_.y = 0;
			velocity_.x = 0;
			velocity_.z = 10;
		}

		if (collider_.collision_[WALL].isBackHit_) {
			velocity_.y = 0;
			velocity_.x = 0;
			velocity_.z = -10;
		}

	}
	else{
		//velocity_.y -= 1.5f * (1.0f / 60.0f);
	}

	renderItem_.Update();
}

void PlayerBullet::Draw() {
	model_->Draw(renderItem_);
}