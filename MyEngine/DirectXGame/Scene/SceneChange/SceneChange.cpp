#include "SceneChange.h"

/// <summary>
/// SceneChange.cpp
/// 画面遷移
/// </summary>

SceneChange* SceneChange::GetInstance() {
	static SceneChange instance;
	return &instance;
}

void SceneChange::Inialize(){
	//カメラ
	spriteCamera_ = SpriteCamera::GetInstance();
	spriteCamera_->Initialize();

	whiteTextureInfo_.Initialize();
	whiteTextureInfo_.spriteItem->spriteData_.textureHandle_ = MyEngine::TextureManager::Load("whiteTexture2x2.png");
	whiteTextureInfo_.spriteItem->spriteData_.size_ = { 1280, 720 };
	whiteTextureInfo_.spriteItem->spriteData_.anchorPoint_ = { 0.0f, 0.0f };
	whiteTextureInfo_.spriteItem->materialInfo_.blendMode_ = BlendMode::kBlendModeNormal;

	isStart_ = false;

	gameScore_ = 0;
}

void SceneChange::Update(){
	spriteCamera_->Update();

	if (isStart_) { //ホワイトイン処理
		time_ += 1.0f / 60.0f;
		if (isReverse_) {
			whiteTextureInfo_.spriteItem->materialInfo_.material_->color.w -= 1.0f / 60.0f;

			if (whiteTextureInfo_.spriteItem->materialInfo_.material_->color.w < 0.0f) {
				whiteTextureInfo_.spriteItem->materialInfo_.material_->color.w = 0.0f;
				isStart_ = false;
			}

		}
		else { //ホワイトアウト処理
			whiteTextureInfo_.spriteItem->materialInfo_.material_->color.w += 1.0f / 60.0f;
			if (whiteTextureInfo_.spriteItem->materialInfo_.material_->color.w > 1.0f) {
				whiteTextureInfo_.spriteItem->materialInfo_.material_->color.w = 1.0f;
				isReverse_ = true;
				isChange_ = true;
			}

		}
	}
	else {
		whiteTextureInfo_.spriteItem->materialInfo_.material_->color.w = 0.0f;
	}

}

void SceneChange::Draw(){

	DrawManager::GetInstance()->PushBackForegroundSprite(&whiteTextureInfo_);

}

void SceneChange::StartSceneChange() {
	if (!isStart_) {
		isStart_ = true;
		isReverse_ = false;
		isChange_ = false;
		time_ = 0.0f;
		whiteTextureInfo_.spriteItem->materialInfo_.material_->color.w = 0.0f;
	}
}
