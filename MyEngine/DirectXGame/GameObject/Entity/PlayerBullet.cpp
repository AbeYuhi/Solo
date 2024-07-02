#include "PlayerBullet.h"

PlayerBullet::PlayerBullet(){}
PlayerBullet::~PlayerBullet(){}

void PlayerBullet::Initialize() {
	input_ = InputManager::GetInstance();

	model_ = Model::Create("sphere", "sphere.obj");
	renderItem_.Initialize();
	renderItem_.worldTransform_.data_.scale_ *= 0.02f;

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
	const float kDistanceTestObject = 5;
	reticlePos_ = posNear + (mouseDirection * kDistanceTestObject);

	velocity_ = Normalize(reticlePos_ - MainCamera::GetInstance()->GetWorldPos());
	velocity_ *= 5.0f;

	//コライダーの設定
	collider_.Initialize(&renderItem_.worldTransform_.data_.translate_, renderItem_.worldTransform_.data_.scale_, BULLET, true, &velocity_);
	CollisionManager::GetInstance()->AddCollider(&collider_);
}

void PlayerBullet::Update() {

	velocity_.y -= 2.0f * (1.0f / 60.0f);
	renderItem_.worldTransform_.data_.translate_ += velocity_ * (1.0f / 60.0f);

	renderItem_.Update();
}

void PlayerBullet::Draw() {
	model_->Draw(renderItem_);
}