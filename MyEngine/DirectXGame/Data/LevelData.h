#pragma once
#include <string>
#include <memory>
#include <list>
#include <vector>
#include "Math/Vector3.h"
#include "Data/Transform.h"

/// <summary>
/// LevelData.h
/// レベルシーンに必要な情報をまとめているファイル
/// </summary>

enum ObjectType {
	kMESH,
	kLight,
	kCamera,
};

struct GlassInfo {
	bool groundingInfosUp;
	bool groundingInfosDown;
	bool groundingInfosLeft;
	bool groundingInfosRight;

	std::string moveType;
	unsigned int verticalDivisions;
	unsigned int horizontalDivisions;
	float moveLimit;
	float moveSpeed;
};

struct WallInfo {
	std::string moveType;
	float moveLimit;
	float moveSpeed;
	float rotateSpeed;
};

struct CameraData {
	EulerTransformData CameraInfo;
	float cameraSpeed;
	Vector3 cameraRotateSpeed;
	Vector3 stageSize;
};

struct LevelData {
	struct ObjectCollider {
		std::string type;
		std::string tag;
		Vector3 centerPos;
		Vector3 rotate;
		Vector3 size;
		float radius;
		bool collisionCheck;

		GlassInfo glassInfo;
		WallInfo wallInfo;
	};
	struct ObjectData {
		Vector3 translation;
		Vector3 rotation;
		Vector3 scaling;
		
		std::string objName;
		std::string fileName;
		bool drawCheck;
		std::optional<ObjectCollider> collider;
		std::optional<int32_t> parent;
		std::optional<CameraData> cameraData;
		ObjectType type;
	};

	std::vector<ObjectData> objects;
};