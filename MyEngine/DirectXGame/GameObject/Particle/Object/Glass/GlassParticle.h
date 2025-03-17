#pragma once
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/AABB.h"
#include "Math/Math.h"

struct Triangle {
	Vector3 pos[3];
};

class GlassParticle
{
public:
	GlassParticle() = default;
	~GlassParticle() = default;

	void Initialize(EulerTransformData* glassData);

	void CreateDelaunayDiagram();


private:
	EulerTransformData* glassData_;


};

