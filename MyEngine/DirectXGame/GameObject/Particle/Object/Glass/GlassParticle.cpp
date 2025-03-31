#include "GlassParticle.h"
#include "GameObject/Entity/Glass.h"

void GlassParticle::Initialize(EulerTransformData* glassPieceData) {
    glassPieceData_ = glassPieceData;
    for (int i = 0; i < 1; i++) {
        lines_[i] = MyEngine::LineObj::Create();
        lines_[i]->Initialize();
    }
}

void GlassParticle::Create(const std::vector<Vector3>& points) {
    CreateDelaunayDiagram(points);
    CreateVoronoiDiagram();
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
        std::vector<Edge> polygon;

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
        for (const Edge& e : polygon) {
            triangles_.push_back({ e.start, e.end, p });
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

void GlassParticle::AddBoundaryEdges(const Triangle& triangle, std::vector<Edge>& polygon) {
    // 三角形の辺を生成
    Edge edges[3] = {
        { triangle.a, triangle.b },
        { triangle.b, triangle.c },
        { triangle.c, triangle.a }
    };

    for (const Edge& e : edges) {
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

Vector3 GlassParticle::ComputeCircumcenter(const Triangle& t) {
    float ax = t.a.x, ay = t.a.y;
    float bx = t.b.x, by = t.b.y;
    float cx = t.c.x, cy = t.c.y;

    float d = 2 * (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by));
    float ux = ((ax * ax + ay * ay) * (by - cy) + (bx * bx + by * by) * (cy - ay) +
        (cx * cx + cy * cy) * (ay - by)) /
        d;
    float uy = ((ax * ax + ay * ay) * (cx - bx) + (bx * bx + by * by) * (ax - cx) +
        (cx * cx + cy * cy) * (bx - ax)) /
        d;

    float uz = t.a.z;

    return Vector3(ux, uy, uz);
}

void GlassParticle::CreateVoronoiDiagram() {

    // 各三角形の外接円の中心を計算
    std::vector<Vector3> circumcenters;
    for (const auto& triangle : triangles_) {
        circumcenters.push_back(ComputeCircumcenter(triangle));
    }

    // 各三角形のペア間で共有エッジを確認し、ボロノイの境界を構築
    for (size_t i = 0; i < triangles_.size(); ++i) {
        for (size_t j = i + 1; j < triangles_.size(); ++j) {
            // 共有する頂点を確認
            int sharedVertices = 0;
            Vector3 shared[2];

            if (triangles_[i].a.x == triangles_[j].a.x && triangles_[i].a.y == triangles_[j].a.y) {
                if (sharedVertices < 2) { // 安全チェック
                    shared[sharedVertices++] = triangles_[i].a;
                }
            }
            if (triangles_[i].b.x == triangles_[j].b.x && triangles_[i].b.y == triangles_[j].b.y) {
                if (sharedVertices < 2) { // 安全チェック
                    shared[sharedVertices++] = triangles_[i].b;
                }
            }
            if (triangles_[i].c.x == triangles_[j].c.x && triangles_[i].c.y == triangles_[j].c.y) {
                if (sharedVertices < 2) { // 安全チェック
                    shared[sharedVertices++] = triangles_[i].c;
                }
            }

            // 2つの頂点を共有している場合、それは共有エッジ
            if (sharedVertices == 2) {
                Edge edge(shared[0], shared[1]);
                voronoiEdges_[edge].push_back(circumcenters[i]);
                voronoiEdges_[edge].push_back(circumcenters[j]);
            }
        }
    }
}

void GlassParticle::Draw() {
    // ボロノイセルの辺を出力（または描画）
    int i = 0;
    for (const auto& [edge, centers] : voronoiEdges_) {
        if (centers.size() == 2) { // 隣接する外接円の中心を結ぶ
            lines_[i]->Draw(centers[0], centers[1]);
            i++;
        }
    }
}