#include "Player.h"

void Player::Initialize(EulerTransformData* cameraData) {
	numberofSlashAttacks_ = 25;
	gameOverTime_ = 0.0f;
	comboDestroyCount_ = 0;
	doorInvincibilityTime_ = 0.0f;
	glassInvincibilityTime_ = 0.0f;
	cameraData_ = cameraData;

	numberSpriteTextures_[0] = MyEngine::TextureManager::Load("numberTexture/0.png");
	numberSpriteTextures_[1] = MyEngine::TextureManager::Load("numberTexture/1.png");
	numberSpriteTextures_[2] = MyEngine::TextureManager::Load("numberTexture/2.png");
	numberSpriteTextures_[3] = MyEngine::TextureManager::Load("numberTexture/3.png");
	numberSpriteTextures_[4] = MyEngine::TextureManager::Load("numberTexture/4.png");
	numberSpriteTextures_[5] = MyEngine::TextureManager::Load("numberTexture/5.png");
	numberSpriteTextures_[6] = MyEngine::TextureManager::Load("numberTexture/6.png");
	numberSpriteTextures_[7] = MyEngine::TextureManager::Load("numberTexture/7.png");
	numberSpriteTextures_[8] = MyEngine::TextureManager::Load("numberTexture/8.png");
	numberSpriteTextures_[9] = MyEngine::TextureManager::Load("numberTexture/9.png");
	
	numberSprites_[0] = MyEngine::Sprite::Create();
	numberSprites_[1] = MyEngine::Sprite::Create();
	numberSprites_[2] = MyEngine::Sprite::Create();
	numberInfo_[0].Initialize(numberSpriteTextures_[0], {30, 50});
	numberInfo_[0].worldTransform_.data_.translate_ = { 90, 50, 0 };
	numberInfo_[1].Initialize(numberSpriteTextures_[0], {30, 50});
	numberInfo_[1].worldTransform_.data_.translate_ = { 60, 50, 0 };
	numberInfo_[2].Initialize(numberSpriteTextures_[0], {30, 50});
	numberInfo_[2].worldTransform_.data_.translate_ = { 30, 50, 0 };

	collider_.Initialize(cameraData_, { .scale_ = {0.1f, 0.1f, 0.1f}, .rotate_ = {0.0f, 0.0f, 0.0f}, .translate_ = {0.0f, 0.0f, 0.0f} }, CAMERA, kOBB, true);
	MyEngine::CollisionManager::GetInstance()->AddCollider(&collider_);

	isShot_ = false;
}

void Player::Update() {
	MyEngine::InputManager* input_ = MyEngine::InputManager::GetInstance();

	if (collider_.isContact_[GOAL]) {
		isGameClear_ = true;
	}

	if (doorInvincibilityTime_ <= 0.0f) {
		if (collider_.isContact_[LDOOR] || collider_.isContact_[RDOOR]) {

			numberofSlashAttacks_ -= 10;
			comboDestroyCount_ = 0;
			doorInvincibilityTime_ = 1.25f;

			isHitEffect_ = true;
			time_ = 0.0f;
		}
	}
	else {
		doorInvincibilityTime_ -= 1.0f / 60.0f;
	}

	if (glassInvincibilityTime_ <= 0.0f) {
		if (collider_.isContact_[GLASS]) {
			numberofSlashAttacks_ -= 10;
			comboDestroyCount_ = 0;
			glassInvincibilityTime_ = 0.5f;

			isHitEffect_ = true;
			time_ = 0.0f;
		}
	}
	else {
		glassInvincibilityTime_ -= 1.0f / 60.0f;
	}

	if (isHitEffect_) {
		time_ += 1.0f / 60.0f;
		MyEngine::PostEffectManager::GetInstance()->SetPostEffect(kVignetteBlur);
		if (MyEngine::RandomManager::GetInstance()->GetRandomNumber(0, 1) == 1) {
			MainCamera::GetInstance()->transform_.rotate_.z += 0.01f;
		}
		else {
			MainCamera::GetInstance()->transform_.rotate_.z -= 0.01f;
		}
		MainCamera::GetInstance()->Update();
		
		if (time_ <= 0.5f) {
			MyEngine::PostEffectManager::GetInstance()->GetVignetteBlurInfo()->intensity += 0.005f;
			MyEngine::PostEffectManager::GetInstance()->GetVignetteBlurInfo()->blurAmount += 0.001f;
		}
		else {
			MyEngine::PostEffectManager::GetInstance()->GetVignetteBlurInfo()->intensity -= 0.005f;
			MyEngine::PostEffectManager::GetInstance()->GetVignetteBlurInfo()->blurAmount -= 0.001f;
		}

		if (time_ >= 1.0f) {
			isHitEffect_ = false;
			MyEngine::PostEffectManager::GetInstance()->SetPostEffect(kNone);
			MainCamera::GetInstance()->transform_.rotate_.z = 0.0f;
			MyEngine::PostEffectManager::GetInstance()->GetVignetteBlurInfo()->intensity = 0.5f;
			MyEngine::PostEffectManager::GetInstance()->GetVignetteBlurInfo()->blurAmount = 0.5f;
		}
	}
	else {
		MainCamera::GetInstance()->transform_.rotate_.z = 0.0f;
	}
	
	if (isShot_) {
		if (numberofSlashAttacks_ > 0) {
			if (input_->IsMouseTrigger(0)) {
				std::unique_ptr<PlayerBullet> bullet = std::make_unique<PlayerBullet>();
				Vector2 mousePos = input_->GetMousePos();
				bullet->Initialize(mousePos);
				bullets_.push_back(std::move(bullet));

				numberofSlashAttacks_--;
			}
		}
	}

	bullets_.remove_if([](auto& bullet) {
		bullet->Update();

		if (bullet->GetLifeTime() <= 0.0f) {
			return true;
		}
		else {
			return false;
		}
		});

	if (numberofSlashAttacks_ <= 0) {
		gameOverTime_ += 1.0f / 60.0f;
		if (gameOverTime_ >= 0.5f) {
			isGameOver_ = true;
		}
	}
	else {
		gameOverTime_ = 0.0f;
		isGameOver_ = false;
	}

	// もし number が 0 の場合、桁数は 1 です。
	digitCount_ = 0;
	int number = numberofSlashAttacks_;
	if (number == 0) {
		digitCount_ = 1;
	}
	else {
		// 負の数の場合は正の数に変換
		number = abs(number);
		while (number > 0) {
			number /= 10;
			digitCount_++;
		}
	}

	if (digitCount_ == 1) {
		numberInfo_[0].spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(numberofSlashAttacks_, 0)];
	}
	else if (digitCount_ == 2) {
		numberInfo_[1].spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(numberofSlashAttacks_, 1)];
		numberInfo_[0].spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(numberofSlashAttacks_, 0)];
	}
	else if (digitCount_ == 3) {
		numberInfo_[2].spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(numberofSlashAttacks_, 2)];
		numberInfo_[1].spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(numberofSlashAttacks_, 1)];
		numberInfo_[0].spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(numberofSlashAttacks_, 0)];
	}

#ifdef _DEBUG
	ImGui::Begin("Combo");
	ImGui::Text("ComboNum : %d", comboDestroyCount_);
	ImGui::End();
#endif // _DEBUG

}

void Player::Draw() {
	for (auto& bullet : bullets_) {
		bullet->Draw();
	}

	if (digitCount_ == 1) {
		numberSprites_[0]->Draw(numberInfo_[0]);
	}
	else if (digitCount_ == 2) {
		numberSprites_[1]->Draw(numberInfo_[1]);
		numberSprites_[0]->Draw(numberInfo_[0]);
	}
	else if (digitCount_ == 3) {
		numberSprites_[2]->Draw(numberInfo_[2]);
		numberSprites_[1]->Draw(numberInfo_[1]);
		numberSprites_[0]->Draw(numberInfo_[0]);
	}

}

void Player::ParticleDraw() {

	for (auto& bullet : bullets_) {
		bullet->ParticleDraw();
	}

}