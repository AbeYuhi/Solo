#include "GlassParticle.h"
#include "GameObject/Entity/Glass.h"

void GlassParticle::Initialize(EulerTransformData* glassPieceData) {
    glassPieceData_ = glassPieceData;
}

void GlassParticle::CreateDelaunayDiagram(const std::vector<Vector3>& points) {
    // スーパー三角形を作成
    Triangle superTriangle = { 
        { glassPieceData_->translate_.x - glassPieceData_->scale_.x, glassPieceData_->translate_.y - glassPieceData_->scale_.y, glassPieceData_->translate_.z},
        { glassPieceData_->translate_.x + (2.0f * glassPieceData_->scale_.x), glassPieceData_->translate_.y - glassPieceData_->scale_.y, glassPieceData_->translate_.z },
        { glassPieceData_->translate_.x - glassPieceData_->scale_.x, glassPieceData_->translate_.y + (2.0f * glassPieceData_->scale_.y), glassPieceData_->translate_.z }};
    triangles_.push_back(superTriangle);

    for (const Vector3& p : points) {
        std::vector<Triangle> badTriangles;
        std::vector<Segment> polygon;

        // 外接円条件を満たさない三角形を探す
        for (const Triangle& t : triangles_) {
            if (InCircumcircle({ p.x, p.y }, t)) {
                badTriangles.push_back(t);
            }
        }

        // 悪い三角形を削除し、その境界エッジを収集
        for (const Triangle& t : badTriangles) {
            // 境界エッジをpolygonに追加（重複チェックを行う）
            AddBoundaryEdges(t, polygon);
        }
        triangles_.erase(
            std::remove_if(triangles_.begin(), triangles_.end(),
                [&badTriangles](const Triangle& t) {
                    return std::find(badTriangles.begin(), badTriangles.end(), t) != badTriangles.end();
                }),
            triangles_.end());

        // 境界から新しい三角形を生成
        for (const Segment& e : polygon) {
            triangles_.push_back({ e.origin, e.origin + e.diff, p });
        }
    }

    // スーパー三角形に関連する三角形を削除
    triangles_.erase(
        std::remove_if(triangles_.begin(), triangles_.end(),
            [&superTriangle](const Triangle& t) {
                return t.contains(superTriangle.a) || t.contains(superTriangle.b) || t.contains(superTriangle.c);
            }),
        triangles_.end());
}

bool GlassParticle::InCircumcircle(const Vector2& p, const Triangle& t) {
    float ax = t.a.x - p.x;
    float ay = t.a.y - p.y;
    float bx = t.b.x - p.x;
    float by = t.b.y - p.y;
    float cx = t.c.x - p.x;
    float cy = t.c.y - p.y;

    float det = (ax * ax + ay * ay) * (bx * cy - cx * by) -
        (bx * bx + by * by) * (ax * cy - cx * ay) +
        (cx * cx + cy * cy) * (ax * by - bx * ay);

    return det > 0; // 外接円内か確認
}

void GlassParticle::AddBoundaryEdges(const Triangle& triangle, std::vector<Segment>& polygon) {
    // 三角形の辺を生成
    Segment edges[3] = {
        { triangle.a, triangle.b },
        { triangle.b, triangle.c },
        { triangle.c, triangle.a }
    };

    for (const Segment& e : edges) {
        auto it = std::find(polygon.begin(), polygon.end(), e);
        if (it != polygon.end()) {
            // 重複する場合は削除（内部の共有エッジ）
            polygon.erase(it);
        }
        else {
            // 重複していない場合は追加
            polygon.push_back(e);
        }
    }
}