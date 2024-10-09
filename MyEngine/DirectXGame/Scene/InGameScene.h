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
#include "Object/SkyBox.h"
#include "Object/WireFrameSphere.h"
#include "GameObject/Camera/MainCamera.h"
#include "GameObject/Camera/InGameCamera.h"
#include "GameObject/Camera/DebugCamera.h"
#include "GameObject/Camera/SpriteCamera.h"
#include "GameObject/Shadow/Shadow.h"
#include "GameObject/Particle/testParticle.h"
#include "GameObject/Particle/Object/PlaneParticle.h"
#include "GameObject/Particle/Sprite/SpriteParticle.h"
#include "GameObject/Entity/Player.h"
#include "LoadFile/LevelScene.h"
#include "Scene/IScene.h"

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
	CollisionManager* collisionManager_;
	MainCamera* mainCamera_;
	SpriteCamera* spriteCamera_;
	//ライト
	LightObjectManager* lightObj_;
	//シャドウ
	std::unique_ptr<Shadow> shadow_;

	//ブレンドモード
	int blendMode_;

	//テクスチャハンドル

	LevelScene levelScene_;
	float time_;

	Player player_;

	//ゲームカメラ
	std::unique_ptr<InGameCamera> gameCamera_;

};