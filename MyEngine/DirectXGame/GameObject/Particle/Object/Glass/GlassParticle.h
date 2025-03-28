#pragma once
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/AABB.h"
#include "Math/Math.h"

struct Triangle {
	Vector3 a, b, c;
	Triangle(Vector3 a, Vector3 b, Vector3 c) : a(a), b(b), c(c) {}
};

struct Edge {
    Vector2 start, end;
    Edge(Vector2 s, Vector2 e) : start(s), end(e) {}

    bool operator<(const Edge& other) const {
        // ソートのための比較演算子（順序を考慮してエッジを一意に識別）
        if (start.x != other.start.x) return start.x < other.start.x;
        if (start.y != other.start.y) return start.y < other.start.y;
        if (end.x != other.end.x) return end.x < other.end.x;
        return end.y < other.end.y;
    }
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
