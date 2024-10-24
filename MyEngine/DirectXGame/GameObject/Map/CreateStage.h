#pragma once
#include "DirectXGame/LoadFile/LoadCSV.h"
#include "DirectXGame/Manager/ImGuiManager.h"
#include "DirectXGame/Manager/TextureManager.h"
#include "DirectXGame/Data/StageBlock.h"

/// <summary>
/// CreateStage.h
/// CSVで読みんだ情報からステージを作成するファイル
/// </summary>

std::list<std::unique_ptr<StageBlock>> CreateStage(std::string stageFileName, Vector3 size);
