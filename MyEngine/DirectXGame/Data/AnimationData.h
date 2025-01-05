#pragma once
#include <string>
#include <vector>
#include "Math/Matrix4x4.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Quaternion.h"
#include "Math/Math.h"

/// <summary>
/// AnimationData.h
/// アニメーションをするにあたって必要な情報を格納するための構造体があるファイル
/// </summary>

template <typename tValue>
struct Keyframe {
	tValue value;
	float time;
};

using KeyframeVector3 = Keyframe<Vector3>;
using KeyframeQuaternion = Keyframe<Quaternion>;

template <typename tValue>
struct AnimationCurve {
	std::vector<Keyframe<tValue>> keyframes;
};

struct NodeAnimation {
	std::string nodeName;
	bool isMeshNode;
	AnimationCurve<Vector3> position;
	AnimationCurve<Quaternion> rotation;
	AnimationCurve<Vector3> scale;
};

struct AnimationData {
	std::string name;
	float duration;
	bool isNodeAnimation;
	std::vector<NodeAnimation> nodeAnimations;
};

struct AnimationInfo {
	AnimationData data;
	bool isAnimation;
	bool preIsAnimation;
	float animationTime;
	float animationSpeed;
	bool isLoop;
};

/// <summary>
/// Vector3の線形補間する処理
/// </summary>
/// <param name="curve"></param>
/// <param name="time"></param>
/// <returns></returns>
Vector3 CalculateValue(const AnimationCurve<Vector3>& curve, float time);

/// <summary>
/// Quaternionの線形補間する処理
/// </summary>
/// <param name="curve"></param>
/// <param name="time"></param>
/// <returns></returns>
Quaternion CalculateValue(const AnimationCurve<Quaternion>& curve, float time);
