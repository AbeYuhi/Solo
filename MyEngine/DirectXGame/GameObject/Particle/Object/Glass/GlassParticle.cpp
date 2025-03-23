#include "GlassParticle.h"
#include "GameObject/Entity/Glass.h"

void GlassParticle::Initialize() {
}

void GlassParticle::CreateDelaunayDiagram(const std::vector<Vector3>& points) {
    // 点群全体を走査
    for (size_t i = 0; i < points.size(); ++i) {
        for (size_t j = i + 1; j < points.size(); ++j) {
            for (size_t k = j + 1; k < points.size(); ++k) {
                Triangle candidate(points[i], points[j], points[k]);

                bool valid = true;
                for (size_t l = 0; l < points.size(); ++l) {
                    if (l != i && l != j && l != k && InCircumcircle({ points[l].x, points[l].y}, candidate)) {
                        valid = false;
                        break;
                    }
                }

                if (valid) {
                    triangles_.push_back(candidate);
                }
            }
        }
    }
}

bool GlassParticle::InCircumcircle(const Vector2& p, const Triangle& t) {
    double ax = t.a.x - p.x;
    double ay = t.a.y - p.y;
    double bx = t.b.x - p.x;
    double by = t.b.y - p.y;
    double cx = t.c.x - p.x;
    double cy = t.c.y - p.y;

    double det = (ax * ax + ay * ay) * (bx * cy - cx * by) -
        (bx * bx + by * by) * (ax * cy - cx * ay) +
        (cx * cx + cy * cy) * (ax * by - bx * ay);

    return det > 0; // 外接円内か確認
}