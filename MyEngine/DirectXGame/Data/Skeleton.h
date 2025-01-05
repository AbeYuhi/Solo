#pragma once
#include <optional>
#include <vector>
#include <stdint.h>
#include <map>
#include "Data/Transform.h"
#include "Data/Node.h"
#include "Math/Matrix4x4.h"

/// <summary>
/// Skeleton.h
/// ボーン情報を収納するための構造体が入っているファイル
/// </summary>

struct Joint {
	QuaternionTransformData transform;
	Matrix4x4 localMatrix;
	Matrix4x4 skeletonSpaceMatrix;
	std::string name;
	std::vector<int32_t> children;
	int32_t index;
	std::optional<int32_t> parent;
};

struct Skeleton {
	int32_t root;
	std::map<std::string, int32_t> jointMap;
	std::vector<Joint> joints;
};

/// <summary>
/// ジョイントの作成
/// </summary>
/// <param name="rootNode"></param>
/// <param name="node"></param>
/// <param name="parent"></param>
/// <param name="joints"></param>
/// <returns></returns>
int32_t CreateJoint(const RootNode& rootNode, const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);

/// <summary>
/// スケルトンの作成
/// </summary>
/// <param name="rootNode"></param>
/// <returns></returns>
Skeleton CreateSkeleton(const RootNode& rootNode);
