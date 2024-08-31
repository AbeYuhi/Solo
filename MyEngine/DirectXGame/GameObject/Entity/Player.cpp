#include "Player.h"

void Player::Initialize() {
	numberofSlashAttacks_ = 10;
	gameOverTime_ = 0.0f;
}

void Player::Update() {
	InputManager* input_ = InputManager::GetInstance();

	if (numberofSlashAttacks_ > 0) {
		if (input_->IsMouseTrigger(0)) {
			std::unique_ptr<PlayerBullet> bullet = std::make_unique<PlayerBullet>();
			bullet->Initialize();
			bullets_.push_back(std::move(bullet));

			numberofSlashAttacks_--;
		}
	}

	if (numberofSlashAttacks_ <= 0) {
		gameOverTime_ += 1.0f / 60.0f;
		if (gameOverTime_ >= 2.0f) {
			isGameOver_ = true;
		}
	}
	else {
		gameOverTime_ = 0.0f;
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
}

void Player::Draw() {
	for (auto& bullet : bullets_) {
		bullet->Draw();
	}
}
