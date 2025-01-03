#pragma once
#include "DirectXGame/Object/Model.h"
#include "DirectXGame/Data/RenderItem.h"
#include "DirectXGame/Data/Collider.h"

/// <summary>
/// StageBlock.h
/// ステージブロック構造体が入っているファイル
/// </summary>

struct StageBlock {
	std::shared_ptr<MyEngine::Model> stageModel;
	MyEngine::RenderItem stageModelInfo;
	Vector3 velocity_;
	std::unique_ptr<Collider> collider;
	int32_t blockInfo;
	bool isBreak;
};