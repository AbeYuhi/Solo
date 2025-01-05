#pragma once
#include <string>
#include <vector>
#include <map>
#include <optional>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include "Math/Matrix4x4.h"
#include "Math/Math.h"
#include "Data/Transform.h"
#include "Data/AnimationData.h"

/// <summary>
/// Node.h
/// ノード情報を格納、読み込むしているファイル
/// </summary>

struct Node {
	QuaternionTransformData transform;
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<int32_t> children;
	int32_t index;
	bool isMove;
};

struct RootNode {
	int32_t rootNode;
	std::vector<Node> nodes;
	std::map<std::string, uint32_t> nodeMap;
};

/// <summary>
/// Nodeの読み込み処理
/// </summary>
/// <param name="node"></param>
/// <param name="nodes"></param>
/// <returns></returns>
int32_t ReadNode(aiNode* node, std::vector<Node>& nodes);

/// <summary>
/// ルートノードの作成処理
/// </summary>
/// <param name="aiNode"></param>
/// <returns></returns>
RootNode CreateRootNode(aiNode* aiNode);

/// <summary>
/// Nodeの更新処理
/// </summary>
/// <param name="node"></param>
/// <param name="info"></param>
/// <param name="time"></param>
/// <returns></returns>
Node UpdateNode(Node node, NodeAnimation info, float time);
