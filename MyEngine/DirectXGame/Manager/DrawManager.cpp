#include "DrawManager.h"

void ModelDrawInfo::Initialize(std::shared_ptr<MyEngine::Model> a_model,
	std::shared_ptr<MyEngine::RenderItem> a_renderItem,
	std::optional<uint32_t> a_textureIndex) {

	model = a_model;
	if (a_renderItem) {
		renderItem = a_renderItem;
	}
	else {
		renderItem = std::make_shared<MyEngine::RenderItem>();
		renderItem->Initialize();
	}
	textureIndex = a_textureIndex;
	isAlive = false;
	isDraw = false;
}

void SpriteDrawInfo::Initialize(std::shared_ptr<MyEngine::SpriteItem> a_spriteItem) {

	sprite = MyEngine::Sprite::Create();
	if (a_spriteItem) {
		spriteItem = a_spriteItem;
	}
	else {
		spriteItem = std::make_shared<MyEngine::SpriteItem>();
		spriteItem->Initialize();
	}
	isAlive = false;
	isDraw = false;
}

DrawManager* DrawManager::GetInstance() {
	static DrawManager instance;
	return &instance;
}

void DrawManager::Draw() {

	//IsDeleteが有効なものを除外
	//背景
	for (auto it = backgroundSprites_.begin(); it != backgroundSprites_.end();) {
		SpriteDrawInfo* info = *it;
		if (!info->isDraw || !info->spriteItem) {
			info->isAlive = false;
			it = backgroundSprites_.erase(it);
		}
		else {
			it++;
		}
	}
	//非透明オブジェクト
	for (auto it = opaqueObjects_.begin(); it != opaqueObjects_.end();) {
		ModelDrawInfo* info = *it;
		if (!info->isDraw || !info->renderItem) {
			info->isAlive = false;
			it = opaqueObjects_.erase(it);
		}
		else {
			it++;
		}
	}
	//透明オブジェクト
	for (auto it = translucentObjects_.begin(); it != translucentObjects_.end();) {
		ModelDrawInfo* info = *it;
		if (!info->isDraw || !info->renderItem) {
			info->isAlive = false;
			it = translucentObjects_.erase(it);
		}
		else {
			it++;
		}
	}

	//半透明オブジェクトを奥からソートする
	std::sort(translucentObjects_.begin(), translucentObjects_.end(),
		[](const ModelDrawInfo* a, const ModelDrawInfo* b) {
			return a->renderItem->worldTransform_.worldPos_.z > b->renderItem->worldTransform_.worldPos_.z; // Z値の降順（遠い順）
		});

	//背景の描画
	for (auto& info : backgroundSprites_) {
		info->sprite->Draw(*info->spriteItem);
	}

	//深度値をクリア
	MyEngine::DirectXCommon::GetInstance()->ClearDepthStencilBuffer();

	//非透明オブジェクトの描画
	for (auto& info : opaqueObjects_) {
		if (info->textureIndex) {
			info->model->Draw(*info->renderItem, *info->textureIndex);
		}
		else {
			info->model->Draw(*info->renderItem);
		}
	}

	//透明オブジェクトを描画
	for (auto& info : translucentObjects_) {
		if (info->textureIndex) {
			info->model->Draw(*info->renderItem, *info->textureIndex);
		}
		else {
			info->model->Draw(*info->renderItem);
		}
	}

	//パーティクルを描画
	for (auto& info :particles_) {
		info.Draw();
	}

	//IsDrawを初期化
	for (auto& info : backgroundSprites_) {
		info->isDraw = false;
	}
	//非透明オブジェクトを初期化
	for (auto& info : opaqueObjects_) {
		info->isDraw = false;
	}
	//透明オブジェクトを初期化
	for (auto& info : translucentObjects_) {
		info->isDraw = false;
	}

	//パーティクルを削除
	particles_.clear();
}

void DrawManager::ForegroundSpritesDraw() {

	//不要なものを削除
	for (auto it = foregroundSprites_.begin(); it != foregroundSprites_.end();) {
		SpriteDrawInfo* info = *it;
		if (!info->isDraw || !info->spriteItem) {
			info->isAlive = false;
			it = foregroundSprites_.erase(it);
		}
		else {
			it++;
		}
	}

	//UIの描画
	for (auto& info : foregroundSprites_) {
		info->sprite->Draw(*info->spriteItem);
	}

	for (auto& info : foregroundSprites_) {
		info->isDraw = false;
	}
}

void DrawManager::Clear() {
	for (auto it = backgroundSprites_.begin(); it != backgroundSprites_.end();) {
		SpriteDrawInfo* info = *it;
		info->isAlive = false;
		info->isDraw = false;
		it = backgroundSprites_.erase(it);
	}
	//非透明オブジェクト
	for (auto it = opaqueObjects_.begin(); it != opaqueObjects_.end();) {
		ModelDrawInfo* info = *it;
		info->isAlive = false;
		info->isDraw = false;
		it = opaqueObjects_.erase(it);
	}
	//透明オブジェクト
	for (auto it = translucentObjects_.begin(); it != translucentObjects_.end();) {
		ModelDrawInfo* info = *it;
		info->isAlive = false;
		info->isDraw = false;
		it = translucentObjects_.erase(it);
	}
	//前衛スプライト
	for (auto it = foregroundSprites_.begin(); it != foregroundSprites_.end();) {
		SpriteDrawInfo* info = *it;
		info->isAlive = false;
		info->isDraw = false;
		it = foregroundSprites_.erase(it);
	}
}

void DrawManager::PushBackOpaqueObject(ModelDrawInfo* info) {
	if (!info->isAlive) {
		opaqueObjects_.push_back(info);
		info->isAlive = true;
	}
	info->isDraw = true;
}

void DrawManager::PushBackTranslucentObject(ModelDrawInfo* info) {
	if (!info->isAlive) {
		translucentObjects_.push_back(info);
		info->isAlive = true;
	}
	info->isDraw = true;
}

void DrawManager::PushBackBackgroundSprite(SpriteDrawInfo* info) {
	if (!info->isAlive) {
		backgroundSprites_.push_back(info);
		info->isAlive = true;
	}
	info->isDraw = true;
}

void DrawManager::PushBackForegroundSprite(SpriteDrawInfo* info) {
	if (!info->isAlive) {
		foregroundSprites_.push_back(info);
		info->isAlive = true;
	}
	info->isDraw = true;
}

void DrawManager::PushBackParticle(MyEngine::Particle particle) {
	particles_.push_back(particle);
}
