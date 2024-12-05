#pragma once
#include <vector>
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
#include "Scene/SceneChange/SceneChange.h"
#include "Scene/IScene.h"

/// <summary>
/// InGameScene.h
/// インゲームシーン
/// </summary>

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
	MyEngine::WinApp* winApp_;
	MyEngine::DirectXCommon* directXCommon_;
	MyEngine::InputManager* input_;
	MyEngine::AudioManager* audioManager_;
	MyEngine::RandomManager* randomManager_;
	MyEngine::PostEffectManager* postEffectManager_;
	MyEngine::CollisionManager* collisionManager_;
	MainCamera* mainCamera_;
	SpriteCamera* spriteCamera_;
	//ライト
	MyEngine::LightObjectManager* lightObj_;
	//シャドウ
	std::unique_ptr<Shadow> shadow_;

	//ブレンドモード
	int blendMode_;

	vector<std::unique_ptr<LevelScene>> levelScenes_;
	float stageSize_;
	unsigned int nowStage_;
	float time_;

	Player player_;

	//ゲームカメラ
	std::unique_ptr<InGameCamera> gameCamera_;

	uint32_t ballShotExplanationTexture_;
	std::shared_ptr<MyEngine::Sprite> ballShotExplanationSprite_;
	MyEngine::SpriteItem ballShotExplanationInfo_;

	uint32_t crystalExplanationTexture_;
	std::shared_ptr<MyEngine::Sprite> crystalExplanationSprite_;
	MyEngine::SpriteItem crystalExplanationInfo_;

	uint32_t gameOverTexture_;
	std::shared_ptr<MyEngine::Sprite> gameOverSprite_;
	MyEngine::SpriteItem gameOverInfo_;
	
	uint32_t backGroundTexture_;
	std::shared_ptr<MyEngine::Sprite> backGroundSprite_;
	MyEngine::SpriteItem backGroundInfo_;

	float startTimer_;
	bool isBallShotRxplanation_;
	float ballShotRxplanationTime_;
	bool isCrystalRxplanation_;
	float crystalRxplanationTime_;
	Vector3 cameraSpeed_;
	float cameraEasingTimer_;
	float easingTimer_;

	bool gameOver_;
	float gameOverTimer_;

	bool gameClear_;
	float gameClearTimer_;

	Vector3 playerSpeed_;
};