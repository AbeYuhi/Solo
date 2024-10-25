#include "IScene.h"

/// <summary>
/// IScene.cpp
/// シーンの親クラス
/// </summary>

int IScene::sceneNo_;
#ifdef _DEBUG
bool IScene::debugMode_ = true;
#else
bool IScene::debugMode_ = false;
#endif // _DEBUG

//共有で使うテクスチャ
uint32_t IScene::uvCheckerHandle_;
uint32_t IScene::whiteTextureHandle_;