#include "GlassParticle.h"

//void GlassParticle::Initialize(EulerTransformData* glassData) {
//    glassData_ = glassData;
//}
//
//void GlassParticle::CreateDelaunayDiagram() {
//
//}
//
//Circle GetCircumscribedCircle(Triangle triangle) {
//
//    Vector3 pos[3];
//    for (int i = 0; i < 3; i++) {
//        pos[i].x = triangle.pos[i].x;
//        pos[i].y = triangle.pos[i].y;
//        pos[i].z = triangle.pos[i].z;
//    }
//
//    float x1_2 = pos[0].x * pos[0].x;
//    float x2_2 = pos[1].x * pos[1].x;
//    float x3_2 = pos[2].x * pos[2].x;
//
//    float y1_2 = pos[0].y * pos[0].y;
//    float y2_2 = pos[1].y * pos[1].y;
//    float y3_2 = pos[2].y * pos[2].y;
//
//    float z1_2 = pos[0].z * pos[0].z;
//    float z2_2 = pos[1].z * pos[1].z;
//    float z3_2 = pos[2].z * pos[2].z;
//
//    // 外接円の中心座標を計算
//    float c = 2 * ((x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1));
//    float x = ((y3 - y1) * (x2_2 - x1_2 + y2_2 - y1_2) + (y1 - y2) * (x3_2 - x1_2 + y3_2 - y1_2)) / c;
//    float y = ((x1 - x3) * (x2_2 - x1_2 + y2_2 - y1_2) + (x2 - x1) * (x3_2 - x1_2 + y3_2 - y1_2)) / c;
//    float _x = (x1 - x);
//    float _y = (y1 - y);
//    float r = std::sqrt((_x * _x) + (_y * _y));
//
//    return Circle(Vector3(x, y, z), r);
//}
//
//
//Triangle getExternalTriangle(AABB aabb) {
//    Vector3 c = { aabb.min.x, aabb.min.y, aabb.min.z };
//    var r = rect.diagonal / 2;
//    var _2r = 2 * r;
//    var _r3r = Math.sqrt(3) * r;
//
//    var A = new Point((cx - _r3r), (cy - r));
//    var B = new Point((cx + _r3r), (cy - r));
//    var C = new Point(cx, (cy + _2r));
//
//    return Triangle([A, B, C]);
//}