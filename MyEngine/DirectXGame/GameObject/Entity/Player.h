#pragma once
#include <memory>
#include <list>
#include "Object/Sprite.h"
#include "PlayerBullet.h"

class Player
{
public:

	void Initialize();

	void Update();

	void Draw();

	inline bool IsGameOver() const { return isGameOver_; }
	inline int* GetNumberofSlashAttacks() { return &numberofSlashAttacks_; }
	inline int* GetComboDestroyCount() { return &comboDestroyCount_; }

private:
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	int numberofSlashAttacks_;
	float gameOverTime_;
	bool isGameOver_;
	int digitCount_;
	int comboDestroyCount_;

	uint32_t numberSpriteTextures_[10];
	std::shared_ptr<Sprite> numberSprites_[3];
	SpriteItem numberInfo_[3];
};

