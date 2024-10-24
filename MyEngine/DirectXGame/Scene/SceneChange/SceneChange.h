#pragma once
#include "Object/Sprite.h"
#include "Data/SpriteItem.h"
#include "Data/SpriteData.h"
#include "GameObject/Camera/SpriteCamera.h"

/// <summary>
/// SceneChange.h
/// 画面遷移
/// </summary>

class SceneChange
{
public:

	static SceneChange* GetInstance();

	void Inialize();

	void Update();

	void Draw();

	void StartSceneChange();

	inline bool IsChange() const { return isChange_; }

private:
	SceneChange() = default;
	~SceneChange() = default;

	SpriteCamera* spriteCamera_;

	uint32_t whiteTexture_;
	std::shared_ptr<Sprite> whiteSprite_;
	SpriteItem whiteInfo_;

	bool isStart_;
	bool isReverse_;
	bool isChange_;
	float time_;
};

