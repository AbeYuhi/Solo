#include "SceneManager.h"

namespace MyEngine {

	SceneManager* SceneManager::GetInstance() {
		static SceneManager instance;
		return &instance;
	}

	void SceneManager::Initialize(GameScene gameScene) {

		//シーンファクトリー
		sceneFactory_ = SceneFactory::GetInstance();
		sceneChange_ = SceneChange::GetInstance();
		sceneChange_->Inialize();

		isChange_ = false;

		//初期シーン
		sceneNo_ = gameScene;
		//初期ゲームシーンの初期化
		scene_ = sceneFactory_->CreateScene((GameScene)sceneNo_);

		//初期シーンの初期化
		scene_->Initialize();
	}

	void SceneManager::Update() {
		preSceneNo_ = sceneNo_;
		sceneNo_ = scene_->GetSceneNo();

#ifdef _DEBUG
		ImGui::Begin("GameScene");
		const char* modes[] = { "Title", "InGame", "Menu", "GameOver", "GameClear" };
		ImGui::Combo("gameScene", &sceneNo_, modes, IM_ARRAYSIZE(modes));
		ImGui::End();
#endif // _DEBUG

		if (preSceneNo_ != sceneNo_) {
			isChange_ = true;
		}

		if (isChange_ && sceneChange_->IsChange()) {
			isChange_ = false;
			//シーンの削除
			scene_->Finalize();
			scene_.release();
			//シーンの生成
			scene_ = sceneFactory_->CreateScene((GameScene)sceneNo_);
			//シーンの初期化
			scene_->Initialize();
		}
		sceneChange_->Update();

		//シーンの更新
		scene_->Update();
	}

	void SceneManager::Draw() {
		//シーンの描画
		scene_->Draw();

		sceneChange_->Draw();
	}

}