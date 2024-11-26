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

	whiteTexture_ = MyEngine::TextureManager::Load("whiteTexture2x2.png");
	whiteSprite_ = MyEngine::Sprite::Create();
	whiteInfo_.Initialize(whiteTexture_, { 1280, 720 }, { 0.0f, 0.0f });

	isStart_ = false;
}

void SceneChange::Update(){
	spriteCamera_->Update();

	if (isStart_) {
		time_ += 1.0f / 60.0f;
		if (isReverse_) {
			whiteInfo_.materialInfo_.material_->color.w -= 1.0f / 60.0f;

			if (whiteInfo_.materialInfo_.material_->color.w <= 0.0f) {
				whiteInfo_.materialInfo_.material_->color.w = 0.0f;
				isStart_ = false;
			}

		}
		else {
			whiteInfo_.materialInfo_.material_->color.w += 1.0f / 60.0f;
			if (whiteInfo_.materialInfo_.material_->color.w >= 1.0f) {
				whiteInfo_.materialInfo_.material_->color.w = 1.0f;
				isReverse_ = true;
				isChange_ = true;
			}

		}
	}

}

void SceneChange::Draw(){

	if (isStart_) {
		whiteSprite_->Draw(whiteInfo_);
	}

}

void SceneChange::StartSceneChange() {
	if (!isStart_) {
		isStart_ = true;
		isReverse_ = false;
		isChange_ = false;
		time_ = 0.0f;
		whiteInfo_.materialInfo_.material_->color.w = 0.0f;
	}
}
