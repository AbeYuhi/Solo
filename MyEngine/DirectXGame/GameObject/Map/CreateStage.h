#pragma once
#include "DirectXGame/LoadFile/LoadCSV.h"
#include "DirectXGame/Manager/ImGuiManager.h"
#include "DirectXGame/Manager/TextureManager.h"
#include "DirectXGame/Data/StageBlock.h"

/// <summary>
/// CreateStage.h
/// CSVで読みんだ情報からステージを作成するファイル
/// </summary>

/// <summary>
/// CSVで読み込んだ情報からステージを作成する処理
/// </summary>
/// <param name="stageFileName"></param>
/// <param name="size"></param>
/// <returns></returns>
std::list<std::unique_ptr<StageBlock>> CreateStage(std::string stageFileName, Vector3 size);
