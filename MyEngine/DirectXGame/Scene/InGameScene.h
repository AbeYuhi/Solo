#pragma once
#include "Base/WinApp/WinApp.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Manager/ImGuiManager.h"
#include "Manager/TextureManager.h"
#include "Manager/InputManager.h"
#include "Manager/AudioManager.h"
#include "Manager/ParticleManager.h"
#include "Manager/PostEffectManager.h"
#include "Manager/RandomManager.h"
#include "Manager/LightObjectManager.h"
#include "Manager/CollisionManager.h"
#include "Object/Sprite.h"
#include "Object/Model.h"
#include "GameObject/Camera/MainCamera.h"
#include "GameObject/Camera/InGameCamera.h"
#include "GameObject/Camera/DebugCamera.h"
#include "GameObject/Camera/SpriteCamera.h"
#include "GameObject/Shadow/Shadow.h"
#include "GameObject/Particle/Object/PlaneParticle.h"
#include "Scene/IScene.h"
#include "GameObject/Entity/PlayerBullet.h"

class InGameScene : public IScene {
public:
	InGameScene();
	~InGameScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:
	//基本機能
	WinApp* winApp_;
	DirectXCommon* directXCommon_;
	InputManager* input_;
	AudioManager* audioManager_;
	RandomManager* randomManager_;
	PostEffectManager* postEffectManager_;
	MainCamera* mainCamera_;
	SpriteCamera* spriteCamera_;
	CollisionManager* collisionManager_;
	//ライト
	LightObjectManager* lightObj_;
	//ゲームカメラ
	std::unique_ptr<InGameCamera> gameCamera_;
	//デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_;
	bool isDebugCamera_;
	//シャドウ
	std::unique_ptr<Shadow> shadow_;

	std::list<PlayerBullet> bullets_;

	std::shared_ptr<Model> boxModel_;
	RenderItem renderItem_;

	//ブレンドモード
	int blendMode_;
};