#include "Player.h"

/// <summary>
/// Player.cpp
/// プレイヤークラスの実装ファイル
/// </summary>

void Player::Initialize(EulerTransformData* cameraData) {
	numberofSlashAttacks_ = 25;
	gameOverTime_ = 0.0f;
	comboDestroyCount_ = 0;
	doorInvincibilityTime_ = 0.0f;
	glassInvincibilityTime_ = 0.0f;
	cameraData_ = cameraData;
	renderItem_.Initialize();
	renderItem_.worldTransform_.data_ = *cameraData;

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
	
	infos_[0].Initialize();
	infos_[1].Initialize();
	infos_[2].Initialize();
	infos_[0].spriteItem->spriteData_.Initialize(numberSpriteTextures_[0], {30, 50});
	infos_[0].spriteItem->worldTransform_.data_.translate_ = { 90, 50, 0 };
	infos_[1].spriteItem->spriteData_.Initialize(numberSpriteTextures_[0], {30, 50});
	infos_[1].spriteItem->worldTransform_.data_.translate_ = { 60, 50, 0 };
	infos_[2].spriteItem->spriteData_.Initialize(numberSpriteTextures_[0], {30, 50});
	infos_[2].spriteItem->worldTransform_.data_.translate_ = { 30, 50, 0 };

	collider_.Initialize(&renderItem_.worldTransform_, { .scale_ = {0.1f, 0.1f, 0.1f}, .rotate_ = {0.0f, 0.0f, 0.0f}, .translate_ = {0.0f, 0.0f, 0.0f} }, CAMERA, kOBB, true);
	MyEngine::CollisionManager::GetInstance()->AddCollider(&collider_);

	isShot_ = false;
}

void Player::Update() {
	MyEngine::InputManager* input_ = MyEngine::InputManager::GetInstance();
	renderItem_.worldTransform_.data_ = *cameraData_;

	//プレイヤーがゴールに衝突したらクリアシーンに移行
	if (collider_.isContact_[GOAL]) {
		isGameClear_ = true;
		MyEngine::CollisionManager::GetInstance()->ClearColliders();
	}

	//ドアと衝突したときの無敵時間
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

	//ガラスと衝突したときの無敵時間
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

	//壁やガラスに衝突したときの演出
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
	
	//弾の発射処理
	if (isShot_) {
		if (numberofSlashAttacks_ > 0) {
			if (input_->IsMouseTrigger(0)) {
				Vector2 mousePos = input_->GetMousePos();

				int bulletNum = (comboDestroyCount_ / 10) + 1;
				if (bulletNum >= 5) {
					bulletNum = 5;
				}
				for (int index = 0; index < bulletNum; index++) {
					Vector2 bulletPos = { 0.0f, 0.0f };
					const int test = 25;

					//コンボ数によって玉の出方を変える処理
					if (bulletNum == 1) {
						bulletPos = mousePos;
					}
					else if (bulletNum == 2) {
						if (index == 0) {
							bulletPos = mousePos;
							bulletPos.x -= test;
						}
						else if (index == 1) {
							bulletPos = mousePos;
							bulletPos.x += test;
						}
					}
					else if (bulletNum == 3) {
						if (index == 0) {
							bulletPos = mousePos;
							bulletPos.y -= test;
						}
						else if (index == 1) {
							bulletPos = mousePos;
							bulletPos.x -= test;
							bulletPos.y += test;
						}
						else if (index == 2) {
							bulletPos = mousePos;
							bulletPos.x += test;
							bulletPos.y += test;
						}
					}
					else if (bulletNum == 4) {
						if (index == 0) {
							bulletPos = mousePos;
							bulletPos.y -= test;
						}
						else if (index == 1) {
							bulletPos = mousePos;
							bulletPos.y += test;
						}
						else if (index == 2) {
							bulletPos = mousePos;
							bulletPos.x -= test;
						}
						else if (index == 3) {
							bulletPos = mousePos;
							bulletPos.x += test;
						}
					}
					else if (bulletNum == 5) {

						if (index == 0) {
							bulletPos = mousePos;
						}
						else if (index == 1) {
							bulletPos = mousePos;
							bulletPos.x -= test;
							bulletPos.y -= test;
						}
						else if (index == 2) {
							bulletPos = mousePos;
							bulletPos.x -= test;
							bulletPos.y += test;
						}
						else if (index == 3) {
							bulletPos = mousePos;
							bulletPos.x += test;
							bulletPos.y -= test;
						}
						else if (index == 4) {
							bulletPos = mousePos;
							bulletPos.x += test;
							bulletPos.y += test;
						}
					}

					std::unique_ptr<PlayerBullet> bullet = std::make_unique<PlayerBullet>();
					bullet->Initialize(bulletPos);
					bullets_.push_back(std::move(bullet));
				}

				numberofSlashAttacks_--;
			}
		}
	}

	//自身の弾の更新処理
	bullets_.remove_if([](auto& bullet) {
		bullet->Update();

		if (bullet->GetLifeTime() <= 0.0f) {
			return true;
		}
		else {
			return false;
		}
		});

	//ゲームオーバーの処理
	if (numberofSlashAttacks_ <= 0) {
		gameOverTime_ += 1.0f / 60.0f;
		const float kGameOverTimeLimit = 0.5f;
		if (gameOverTime_ >= kGameOverTimeLimit) {
			isGameOver_ = true;
		}
	}
	else {
		gameOverTime_ = 0.0f;
		isGameOver_ = false;
	}

	//プレイヤーの残弾を表示するための処理
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
		infos_[0].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(numberofSlashAttacks_, 0)];
	}
	else if (digitCount_ == 2) {
		infos_[1].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(numberofSlashAttacks_, 1)];
		infos_[0].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(numberofSlashAttacks_, 0)];
	}
	else if (digitCount_ == 3) {
		infos_[2].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(numberofSlashAttacks_, 2)];
		infos_[1].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(numberofSlashAttacks_, 1)];
		infos_[0].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(numberofSlashAttacks_, 0)];
	}

#ifdef _DEBUG
	ImGui::Begin("Combo");
	ImGui::Text("ComboNum : %d", comboDestroyCount_);
	ImGui::End();
#endif // _DEBUG

}

void Player::Draw() {

	//弾の描画
	for (auto& bullet : bullets_) {
		bullet->Draw();
	}

	//残り玉数の描画
	if (digitCount_ == 1) {
		DrawManager::GetInstance()->PushBackForegroundSprite(&infos_[0]);
	}
	else if (digitCount_ == 2) {
		DrawManager::GetInstance()->PushBackForegroundSprite(&infos_[1]);
		DrawManager::GetInstance()->PushBackForegroundSprite(&infos_[0]);
	}
	else if (digitCount_ == 3) {
		DrawManager::GetInstance()->PushBackForegroundSprite(&infos_[2]);
		DrawManager::GetInstance()->PushBackForegroundSprite(&infos_[1]);
		DrawManager::GetInstance()->PushBackForegroundSprite(&infos_[0]);
	}

}