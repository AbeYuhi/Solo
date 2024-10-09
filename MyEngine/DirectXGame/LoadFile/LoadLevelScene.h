#pragma once
#include <variant>
#include <map>
#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <optional>
#include "External/nlohmann/json.hpp"
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Manager/ImGuiManager.h"
#include "DirectXGame/Manager/CollisionManager.h"
#include "Object/Model.h"
#include "Data/LevelData.h"
#include "Data/Collider.h"

using json = nlohmann::json;

std::unique_ptr<LevelData>  LoadFile(std::string directoryPath, std::string fileName);

void ScanChildData(LevelData* levelData, json& childrens, int32_t parent);