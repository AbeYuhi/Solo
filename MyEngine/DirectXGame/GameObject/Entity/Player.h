#pragma once
#include <memory>
#include <list>
#include "PlayerBullet.h"

class Player
{
public:

	void Initialize();

	void Update();

	void Draw();

	inline bool IsGameOver() const { return isGameOver_; }

private:
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	int numberofSlashAttacks_;
	float gameOverTime_;
	bool isGameOver_;
};

