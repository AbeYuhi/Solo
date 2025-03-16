RWStructuredBuffer<float32_t2> sites; // サイト情報（点のリスト）
RWStructuredBuffer<uint32_t2> triangles; // 三角形リスト（頂点インデックス）
RWStructuredBuffer<float32_t2> outputTriangles; // 出力用の三角形（頂点座標）

struct Constants
{
    uint numSites; // サイトの数
    uint numTriangles; // 三角形の数
};
ConstantBuffer<Constants> gConstants : register(b0);

struct Triangle
{
    float32_t2 v1;
    float32_t2 v2;
    float32_t2 v3;
};

// 外接円を求める関数
bool InCircumcircle(float2 p, Triangle tri)
{
    float ax = tri.v1.x - p.x;
    float ay = tri.v1.y - p.y;
    float bx = tri.v2.x - p.x;
    float by = tri.v2.y - p.y;
    float cx = tri.v3.x - p.x;
    float cy = tri.v3.y - p.y;

    float det = ax * (by * (cx * cx + cy * cy) - cy * (bx * bx + by * by))
              - ay * (bx * (cx * cx + cy * cy) - cx * (bx * bx + by * by))
              + (ax * ax + ay * ay) * (bx * cy - by * cx);

    return det > 0.0;
}

[numthreads(1024, 1, 1)]
void main(uint32_t threadID : SV_DispatchThreadID)
{
    if (threadID.x >= gConstants.numTriangles)
        return;

    uint index = threadID.x;
    Triangle tri = Triangle(
        sites[triangles[index].x],
        sites[triangles[index].y],
        sites[triangles[index].z]
    );

    // サイトが外接円の内部にあるか判定
    for (uint i = 0; i < gConstants.numSites; i++)
    {
        if (InCircumcircle(sites[i], tri))
        {
            // 三角形を削除（新しい三角形で上書き）
            outputTriangles[index * 3 + 0] = tri.v1;
            outputTriangles[index * 3 + 1] = tri.v2;
            outputTriangles[index * 3 + 2] = tri.v3;
        }
    }
}