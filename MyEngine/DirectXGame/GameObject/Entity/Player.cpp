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

	bulletNumTextures_[0] = MyEngine::TextureManager::Load("bulletNums/bulletNum1.png");
	bulletNumTextures_[1] = MyEngine::TextureManager::Load("bulletNums/bulletNum2.png");
	bulletNumTextures_[2] = MyEngine::TextureManager::Load("bulletNums/bulletNum3.png");
	bulletNumTextures_[3] = MyEngine::TextureManager::Load("bulletNums/bulletNum4.png");
	bulletNumTextures_[4] = MyEngine::TextureManager::Load("bulletNums/bulletNum5.png");
	
	infos_[0].Initialize();
	infos_[1].Initialize();
	infos_[2].Initialize();
	infos_[0].spriteItem->spriteData_.Initialize(numberSpriteTextures_[0], {30, 50});
	infos_[0].spriteItem->worldTransform_.data_.translate_ = { 670, 50, 0 };
	infos_[1].spriteItem->spriteData_.Initialize(numberSpriteTextures_[0], {30, 50});
	infos_[1].spriteItem->worldTransform_.data_.translate_ = { 640, 50, 0 };
	infos_[2].spriteItem->spriteData_.Initialize(numberSpriteTextures_[0], {30, 50});
	infos_[2].spriteItem->worldTransform_.data_.translate_ = { 610, 50, 0 };

	bulletNumInfo_.Initialize();
	bulletNumInfo_.spriteItem->spriteData_.Initialize(bulletNumTextures_[0], { 100, 100 });
	bulletNumInfo_.spriteItem->worldTransform_.data_.translate_ = { 580, 50, 0 };

	bulletComboTextures_ = MyEngine::TextureManager::Load("bulletCombo.png");
	for (int i = 0; i < 9; i++) {
		bulletComboInfo_[i].Initialize();
		bulletComboInfo_[i].spriteItem->spriteData_.Initialize(bulletComboTextures_, { 100, 100 });
		bulletComboInfo_[i].spriteItem->worldTransform_.data_.translate_ = { 580, 50, 0 };
		bulletComboInfo_[i].spriteItem->worldTransform_.data_.rotate_.z += (3.14f / 5.0f) * (i + 1);
	}

	collider_.Initialize(&renderItem_.worldTransform_, { .scale_ = {0.1f, 0.1f, 0.1f}, .rotate_ = {0.0f, 0.0f, 0.0f}, .translate_ = {0.0f, 0.0f, 0.0f} }, CAMERA, kOBB, true);
	MyEngine::CollisionManager::GetInstance()->AddCollider(&collider_);

	//弾の射出処理
	shotStateManager_ = std::make_unique<ShotStateManager>(this);

	missSound_ = MyEngine::AudioManager::GetInstance()->SoundLoadMp3("missSound.mp3");

	isShot_ = false;
	isBallLost_ = false;
}

void Player::Update() {
	MyEngine::InputManager* input = MyEngine::InputManager::GetInstance();
	renderItem_.worldTransform_.data_ = *cameraData_;

	//ドアと衝突したときの無敵時間
	if (doorInvincibilityTime_ <= 0.0f) {
		if (collider_.isContact_[LDOOR] || collider_.isContact_[RDOOR]) {
			//失敗したときに音を鳴らす
			MyEngine::AudioManager::GetInstance()->SoundPlayMp3(missSound_, kMissSoundVolume_);
			numberofSlashAttacks_ -= kComboIncreaseStep;
			comboDestroyCount_ = 0;
			doorInvincibilityTime_ = 1.25f;

			isHitEffect_ = true;
			isBallLost_ = true;
			time_ = 0.0f;
		}
	}
	else {
		doorInvincibilityTime_ -= 1.0f / 60.0f;
	}

	//ガラスと衝突したときの無敵時間
	if (glassInvincibilityTime_ <= 0.0f) {
		if (collider_.isContact_[GLASS]) {
			//失敗したときに音を鳴らす
			MyEngine::AudioManager::GetInstance()->SoundPlayMp3(missSound_, kMissSoundVolume_);
			numberofSlashAttacks_ -= kComboIncreaseStep;
			comboDestroyCount_ = 0;
			glassInvincibilityTime_ = 0.5f;

			isHitEffect_ = true;
			isBallLost_ = true;
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

	//現在のコンボ数に応じて出る弾の量を計算
	int bulletNum = (comboDestroyCount_ / kComboIncreaseStep) + 1;
	//bulletNumに応じてUIが変わるように
	bulletNumInfo_.spriteItem->spriteData_.textureHandle_ = bulletNumTextures_[bulletNum - 1];
	
	shotStateManager_->IncrementCombo();
	//弾の発射処理
	if (isShot_) {
		if (numberofSlashAttacks_ > 0) {
			if (input->IsMouseTrigger(0)) {
				shotStateManager_->Shoot();
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
	//桁数の計算
	digitCount_ = 0;
	int number = numberofSlashAttacks_;
	if (number == 0) {
		digitCount_ = 1;
	}
	else {
		// 負の数の場合は正の数に変換
		number = abs(number);
		while (number > 0) {
			//10進数だから10で割る
			number /= 10;
			digitCount_++;
		}
	}

	//桁数に応じた処理
	if (digitCount_ == 1) {
		infos_[0].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(numberofSlashAttacks_, 0)];
		//玉の桁数により表示位置の変更
		infos_[0].spriteItem->worldTransform_.data_.translate_ = { 655, 50, 0 };
	}
	else if (digitCount_ == 2) {
		infos_[1].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(numberofSlashAttacks_, 1)];
		infos_[0].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(numberofSlashAttacks_, 0)];
		//玉の桁数により表示位置の変更
		infos_[1].spriteItem->worldTransform_.data_.translate_ = { 655, 50, 0 };
		infos_[0].spriteItem->worldTransform_.data_.translate_ = { 685, 50, 0 };
	}
	else if (digitCount_ == 3) {
		infos_[2].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(numberofSlashAttacks_, 2)];
		infos_[1].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(numberofSlashAttacks_, 1)];
		infos_[0].spriteItem->spriteData_.textureHandle_ = numberSpriteTextures_[getDigits(numberofSlashAttacks_, 0)];
		//玉の桁数により表示位置の変更
		infos_[2].spriteItem->worldTransform_.data_.translate_ = { 655, 50, 0 };
		infos_[1].spriteItem->worldTransform_.data_.translate_ = { 685, 50, 0 };
		infos_[0].spriteItem->worldTransform_.data_.translate_ = { 915, 50, 0 };
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

	//コンボ数などの玉の情報の描画
	DrawManager::GetInstance()->PushBackForegroundSprite(&bulletNumInfo_);

	int num = (comboDestroyCount_ / kComboIncreaseStep);
	int combo = comboDestroyCount_ - (num * kComboIncreaseStep);
	for (int i = 0; i < combo; i++) {
		DrawManager::GetInstance()->PushBackForegroundSprite(&bulletComboInfo_[i]);
	}

}

void Player::Shoot(Vector2 bulletPos) {
	std::unique_ptr<PlayerBullet> bullet = std::make_unique<PlayerBullet>();
	bullet->Initialize(bulletPos);
	bullets_.push_back(std::move(bullet));
}

void ShotState1::Shoot(Player* player) {
	MyEngine::InputManager* input = MyEngine::InputManager::GetInstance();
	Vector2 mousePos = input->GetMousePos();

	//コンボ数によって玉の出方を変える
	Vector2 bulletPos = { 0.0f, 0.0f };
	bulletPos = mousePos;
	player->Shoot(bulletPos);
	int* playerNumberofSlashAttacks = player->GetNumberofSlashAttacks();
	*playerNumberofSlashAttacks -= 1;
}

std::unique_ptr<ShotState> ShotState1::NextState() {
	return std::make_unique<ShotState2>();
}

ShotStateType ShotState1::stateNo() {
	return ShotStateType::State1;
}

void ShotState2::Shoot(Player* player) {
	MyEngine::InputManager* input = MyEngine::InputManager::GetInstance();
	Vector2 mousePos = input->GetMousePos();

	for (int index = 0; index < 2; index++) {
		Vector2 bulletPos = { 0.0f, 0.0f };
		bulletPos = mousePos;
		//コンボ数によって玉の出方を変える処理
		if (index == 0) {
			bulletPos = mousePos;
			bulletPos.x -= kBallSpacing;
		}
		else if (index == 1) {
			bulletPos = mousePos;
			bulletPos.x += kBallSpacing;
		}
		player->Shoot(bulletPos);
	}
	int* playerNumberofSlashAttacks = player->GetNumberofSlashAttacks();
	*playerNumberofSlashAttacks -= 1;
}

std::unique_ptr<ShotState> ShotState2::NextState() {
	return std::make_unique<ShotState3>();
}

ShotStateType ShotState2::stateNo() {
	return ShotStateType::State2;
}

void ShotState3::Shoot(Player* player) {
	MyEngine::InputManager* input = MyEngine::InputManager::GetInstance();
	Vector2 mousePos = input->GetMousePos();

	for (int index = 0; index < 3; index++) {
		Vector2 bulletPos = { 0.0f, 0.0f };
		bulletPos = mousePos;
		//コンボ数によって玉の出方を変える処理
		if (index == 0) {
			bulletPos = mousePos;
			bulletPos.y -= kBallSpacing;
		}
		else if (index == 1) {
			bulletPos = mousePos;
			bulletPos.x -= kBallSpacing;
			bulletPos.y += kBallSpacing;
		}
		else if (index == 2) {
			bulletPos = mousePos;
			bulletPos.x += kBallSpacing;
			bulletPos.y += kBallSpacing;
		}
		player->Shoot(bulletPos);
	}
	int* playerNumberofSlashAttacks = player->GetNumberofSlashAttacks();
	*playerNumberofSlashAttacks -= 1;
}

std::unique_ptr<ShotState> ShotState3::NextState() {
	return std::make_unique<ShotState4>();
}

ShotStateType ShotState3::stateNo() {
	return ShotStateType::State3;
}

void ShotState4::Shoot(Player* player) {
	MyEngine::InputManager* input = MyEngine::InputManager::GetInstance();
	Vector2 mousePos = input->GetMousePos();

	for (int index = 0; index < 4; index++) {
		Vector2 bulletPos = { 0.0f, 0.0f };
		bulletPos = mousePos;
		//コンボ数によって玉の出方を変える処理
		if (index == 0) {
			bulletPos = mousePos;
			bulletPos.y -= kBallSpacing;
		}
		else if (index == 1) {
			bulletPos = mousePos;
			bulletPos.y += kBallSpacing;
		}
		else if (index == 2) {
			bulletPos = mousePos;
			bulletPos.x -= kBallSpacing;
		}
		else if (index == 3) {
			bulletPos = mousePos;
			bulletPos.x += kBallSpacing;
		}
		player->Shoot(bulletPos);
	}
	int* playerNumberofSlashAttacks = player->GetNumberofSlashAttacks();
	*playerNumberofSlashAttacks -= 1;
}

std::unique_ptr<ShotState> ShotState4::NextState() {
	return std::make_unique<ShotState5>();
}

ShotStateType ShotState4::stateNo() {
	return ShotStateType::State4;
}

void ShotState5::Shoot(Player* player) {
	MyEngine::InputManager* input = MyEngine::InputManager::GetInstance();
	Vector2 mousePos = input->GetMousePos();

	for (int index = 0; index < 5; index++) {
		Vector2 bulletPos = { 0.0f, 0.0f };
		bulletPos = mousePos;
		//コンボ数によって玉の出方を変える処理
		if (index == 0) {
			bulletPos = mousePos;
		}
		else if (index == 1) {
			bulletPos = mousePos;
			bulletPos.x -= kBallSpacing;
			bulletPos.y -= kBallSpacing;
		}
		else if (index == 2) {
			bulletPos = mousePos;
			bulletPos.x -= kBallSpacing;
			bulletPos.y += kBallSpacing;
		}
		else if (index == 3) {
			bulletPos = mousePos;
			bulletPos.x += kBallSpacing;
			bulletPos.y -= kBallSpacing;
		}
		else if (index == 4) {
			bulletPos = mousePos;
			bulletPos.x += kBallSpacing;
			bulletPos.y += kBallSpacing;
		}
		player->Shoot(bulletPos);
	}
	int* playerNumberofSlashAttacks = player->GetNumberofSlashAttacks();
	*playerNumberofSlashAttacks -= 1;
}

std::unique_ptr<ShotState> ShotState5::NextState() {
	return std::make_unique<ShotState5>();
}

ShotStateType ShotState5::stateNo() {
	return ShotStateType::State5;
}

ShotStateManager::ShotStateManager(Player* player){
	player_ = player;
	currentState_ = std::make_unique<ShotState1>();
	comboSound_ = MyEngine::AudioManager::GetInstance()->SoundLoadMp3("comboSound.mp3");
	shotSound_ = MyEngine::AudioManager::GetInstance()->SoundLoadMp3("shotSound.mp3");
	lastProcessedCombo_ = -1; // 最後に処理したコンボ数を記録
}

void ShotStateManager::IncrementCombo() {
	int* comboCount = player_->GetComboDestroyCount();
	if (*comboCount == 0) {
		if (currentState_->stateNo() != ShotStateType::State1) {
			currentState_ = std::make_unique<ShotState1>();
		}
	}
	else if (*comboCount % player_->kComboIncreaseStep == 0 && *comboCount != lastProcessedCombo_) {
		//10コンボごとに状態遷移、かつ前回処理したコンボ数と異なる場合のみ処理
		std::unique_ptr<ShotState> newState = currentState_->NextState();

		if (newState->stateNo() != currentState_->stateNo()) {
			MyEngine::AudioManager::GetInstance()->SoundPlayMp3(comboSound_, kComboSoundVolume_);
			currentState_.reset();
			currentState_ = std::move(newState);
			lastProcessedCombo_ = *comboCount; // 処理済みのコンボ数を更新
		}
	}
}

void ShotStateManager::Shoot() {
	MyEngine::AudioManager::GetInstance()->SoundPlayMp3(shotSound_, kShotSoundVolume_);
	currentState_->Shoot(player_);
}