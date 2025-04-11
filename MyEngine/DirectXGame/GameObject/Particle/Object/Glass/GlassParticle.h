#pragma once
#include <cmath>
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/AABB.h"
#include "Math/Math.h"
#include "Object/Line.h"

// 浮動小数点数の比較を行うためのイプシロン値
const float EPSILON = 1e-5f;

struct Triangle {
	Vector3 a, b, c;
	Triangle(Vector3 a, Vector3 b, Vector3 c) : a(a), b(b), c(c) {}

	bool contains(const Vector3& p) const {
		return (a == p || b == p || c == p);
	}

	// operator== の定義
	bool operator==(const Triangle& other) const {
		// 三角形の頂点がすべて一致している場合
		return (a.x == other.a.x && a.y == other.a.y &&
			b.x == other.b.x && b.y == other.b.y &&
			c.x == other.c.x && c.y == other.c.y) ||
			// 順不同で一致している場合も考慮
			(a.x == other.b.x && a.y == other.b.y &&
				b.x == other.c.x && b.y == other.c.y &&
				c.x == other.a.x && c.y == other.a.y) ||
			(a.x == other.c.x && a.y == other.c.y &&
				b.x == other.a.x && b.y == other.a.y &&
				c.x == other.b.x && c.y == other.b.y);
	}
};

// Edgeクラスにoperator==を実装
struct Edge {
	Vector3 start;
	Vector3 end;

	// コンストラクタ
	Edge() : start(), end() {}
	Edge(const Vector3& s, const Vector3& e) : start(s), end(e) {}

	bool operator==(const Edge& other) const {
		// 始点と終点が一致、または終点と始点が一致する場合に等しいと判定
		return (start.x == other.start.x && start.y == other.start.y && start.z == other.start.z &&
			end.x == other.end.x && end.y == other.end.y && end.z == other.end.z) ||
			(start.x == other.end.x && start.y == other.end.y && start.z == other.end.z &&
				end.x == other.start.x && end.y == other.start.y && end.z == other.start.z);
	}

	// < 演算子のオーバーロード
	bool operator<(const Edge& other) const {
		// 辞書式順序で比較 (start -> end)
		if (start.x < other.start.x) return true;
		if (start.x > other.start.x) return false;
		if (start.y < other.start.y) return true;
		if (start.y > other.start.y) return false;
		if (start.z < other.start.z) return true;
		if (start.z > other.start.z) return false;

		if (end.x < other.end.x) return true;
		if (end.x > other.end.x) return false;
		if (end.y < other.end.y) return true;
		if (end.y > other.end.y) return false;
		if (end.z < other.end.z) return true;
		return false;
	}
};

class Glass;

class GlassParticle
{
public:
	GlassParticle() = default;
	~GlassParticle() = default;

	void Initialize(EulerTransformData* glassPieceData);

	void Create(const std::vector<Vector3>& points);

	void CreateDelaunayDiagram(const std::vector<Vector3>& points);

	void CreateVoronoiDiagram();

	void Draw();

private:

	bool InCircumcircle(const Vector2& p, const Triangle& t);

	void AddBoundaryEdges(const Triangle& triangle, std::vector<Edge>& polygon);

	Vector3 ComputeCircumcenter(const Triangle& t);

	bool FloatEquals(float a, float b);

private:
	EulerTransformData* glassPieceData_;
	std::vector<Triangle> triangles_;
	std::map<Edge, std::vector<Vector3>> voronoiEdges_;
	std::shared_ptr<MyEngine::LineObj> lines_[100];
};
