#pragma once
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/AABB.h"
#include "Math/Math.h"

struct Triangle {
	Vector3 a, b, c;
	Triangle(Vector3 a, Vector3 b, Vector3 c) : a(a), b(b), c(c) {}
};

class Glass;

class GlassParticle
{
public:
	GlassParticle() = default;
	~GlassParticle() = default;

	void Initialize();

	void CreateDelaunayDiagram(const std::vector<Vector3>& points);

private:

	bool InCircumcircle(const Vector2& p, const Triangle& t);

private:
	std::vector<Triangle> triangles_;


};
