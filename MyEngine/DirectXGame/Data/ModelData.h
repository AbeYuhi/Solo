#pragma once
#include <vector>
#include "VertexData.h"
#include "Node.h"
#include "Material.h"
#include "MaterialData.h"

/// <summary>
/// ModelData.h
/// パーティクルの情報をGPUに送るデータを格納するためのファイル
/// </summary>

struct ModelData {
	std::vector<VertexData> vertices;
	std::vector<uint32_t> indices;
	std::map<std::string, JointWeightData> skinClusterData;
	MaterialData material;
};