#include "GlassParticle.h"

void GlassParticle::Initialize(EulerTransformData* glassData) {
    glassData_ = glassData
}

void GlassParticle::CreateDelaunayDiagram() {

}

/**
 * 外接円を得る
 * @param {Triangle} triangle 外接円を得たい三角形
 * @return {Circle} 外接円
 */
function getCircumscribedCircle(triangle) {

    var x1 = triangle.points[0].x;
    var y1 = triangle.points[0].y;
    var x2 = triangle.points[1].x;
    var y2 = triangle.points[1].y;
    var x3 = triangle.points[2].x;
    var y3 = triangle.points[2].y;

    var x1_2 = x1 * x1;
    var x2_2 = x2 * x2;
    var x3_2 = x3 * x3;
    var y1_2 = y1 * y1;
    var y2_2 = y2 * y2;
    var y3_2 = y3 * y3;

    // 外接円の中心座標を計算
    var c = 2 * ((x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1));
    var x = ((y3 - y1) * (x2_2 - x1_2 + y2_2 - y1_2) + (y1 - y2) * (x3_2 - x1_2 + y3_2 - y1_2)) / c;
    var y = ((x1 - x3) * (x2_2 - x1_2 + y2_2 - y1_2) + (x2 - x1) * (x3_2 - x1_2 + y3_2 - y1_2)) / c;
    var _x = (x1 - x);
    var _y = (y1 - y);
    var r = Math.sqrt((_x * _x) + (_y * _y));

    return new Circle(new Point(x, y), r);
}


Triangle getExternalTriangle(AABB aabb) {
    Vector3 c = { aabb.min.x, aabb.min.y, aabb.min.z };
    var r = rect.diagonal / 2;
    var _2r = 2 * r;
    var _r3r = Math.sqrt(3) * r;

    var A = new Point((cx - _r3r), (cy - r));
    var B = new Point((cx + _r3r), (cy - r));
    var C = new Point(cx, (cy + _2r));

    return Triangle([A, B, C]);
}