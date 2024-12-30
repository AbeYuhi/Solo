#pragma once
#include <optional>
#include <vector>
#include <memory>
#include <variant>
#include "Data/RenderItem.h"
#include "Data/SpriteData.h"
#include "Data/SpriteItem.h"
#include "Manager/ParticleManager.h"
#include "GameObject/Particle/Object/GlassPieceParticle/GlassPieceParticle.h"
#include "Object/Model.h"
#include "Object/Sprite.h"

struct ModelDrawInfo {
	std::shared_ptr<MyEngine::Model> model;
	std::shared_ptr<MyEngine::RenderItem> renderItem;
	std::optional<uint32_t> textureIndex;
	bool isAlive;
	bool isDraw;

	void Initialize(std::shared_ptr<MyEngine::Model> a_model,
		std::shared_ptr<MyEngine::RenderItem> a_renderItem = nullptr,
		std::optional<uint32_t> a_textureIndex = std::nullopt);
};

struct SpriteDrawInfo {
	std::shared_ptr<MyEngine::Sprite> sprite;
	std::shared_ptr<MyEngine::SpriteItem> spriteItem;
	bool isAlive;
	bool isDraw;

	void Initialize(std::shared_ptr<MyEngine::SpriteItem> a_spriteItem = nullptr);
};

class DrawManager
{
public: //静的関数
	static DrawManager* GetInstance();
public:

	/// <summary>
	/// 通常の描画関数
	/// </summary>
	void Draw();

	/// <summary>
	/// 前衛スプライトを描画する関数
	/// </summary>
	void ForegroundSpritesDraw();

	/// <summary>
	/// 要素をすべて消す
	/// </summary>
	void Clear();

	/// <summary>
	/// 非透明オブジェクトの要素を入れる
	/// </summary>
	/// <param name="model">3DModel</param>
	/// <param name="renderItem">レンダリング情報</param>
	/// <param name="textureIndex">テクスチャ</param>
	void PushBackOpaqueObject(ModelDrawInfo* info);
	
	/// <summary>
	/// 半透明オブジェクトの要素を入れる
	/// </summary>
	/// <param name="model">3DModel</param>
	/// <param name="renderItem">レンダリング情報</param>
	/// <param name="textureIndex">テクスチャ</param>
	void PushBackTranslucentObject(ModelDrawInfo* info);
	
	/// <summary>
	/// 背景スプライトの要素を入れる
	/// </summary>
	/// <param name="sprite">Sprite</param>
	/// <param name="spriteItem">Spriteの情報</param>
	void PushBackBackgroundSprite(SpriteDrawInfo* info);

	/// <summary>
	/// 前衛スプライトの要素を入れる
	/// </summary>
	/// <param name="sprite">Sprite</param>
	/// <param name="spriteItem">Spriteの情報</param>
	void PushBackForegroundSprite(SpriteDrawInfo* info);

	/// <summary>
	/// パーティクルの要素を入れる
	/// </summary>
	/// <param name="sprite">Sprite</param>
	/// <param name="spriteItem">Spriteの情報</param>
	void PushBackParticle(MyEngine::Particle particle);

private:

	// 非透明オブジェクトのリスト
	std::vector<ModelDrawInfo*> opaqueObjects_;

	// 半透明オブジェクトのリスト
	std::vector<ModelDrawInfo*> translucentObjects_;

	// 背景スプライト
	std::vector<SpriteDrawInfo*> backgroundSprites_;

	// 前衛スプライト
	std::vector<SpriteDrawInfo*> foregroundSprites_;

	//パーティクル
	std::vector<MyEngine::Particle> particles_;

};

