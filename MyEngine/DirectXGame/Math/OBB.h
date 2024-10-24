#pragma once
#include "Vector3.h"
#include "Sphere.h"
#include "Matrix4x4.h"

/// <summary>
/// OBB.h
/// OBB構造体のデータファイル
/// </summary>

struct OBB
{
	Vector3 center;
	Vector3 orientations[3];
	Vector3 size;
};

