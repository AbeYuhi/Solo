#pragma once
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include "Math/Matrix4x4.h"
#include "Node.h"

/// <summary>
/// Bone.h
/// アニメーションに必要なBoneの情報を収納する構造体があるファイル
/// </summary>

struct Bone final {
	std::string name;
	Matrix4x4 localMatrix;
	size_t boneNum;
};
