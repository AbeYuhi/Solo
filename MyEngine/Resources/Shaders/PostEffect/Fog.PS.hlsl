#include "FullScreen.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
Texture2D<float> gDepthTexture : register(t1);
SamplerState gSampler : register(s0);
SamplerState gDepthSampler : register(s1);

struct MaterialInfo
{
    float4x4 inverseProjectionMatrix;
};
ConstantBuffer<MaterialInfo> gMaterial : register(b0);

struct PixelShaderOutput
{
    float32_t4 color : SV_Target0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    // 深度値を取得
    float ndcDepth = gDepthTexture.Sample(gDepthSampler, input.texcoord);
    // 深度値をクリップ空間のZに変換
    float clipZ = ndcDepth * 2.0f - 1.0f;
    // クリップ空間のZ値をビュー空間に変換
    float4 viewSpace = mul(float32_t4(0.0f, 0.0f, clipZ, 1.0f), gMaterial.inverseProjectionMatrix);
    // ビュー空間のZ値を取得
    float viewZ = viewSpace.z * rcp(viewSpace.w);
    
    //全体的な霧の濃さ
    float CONSTANT_FOG_SCALE = 1.0f;
    //霧が濃くなるスピード
    float CONSTANT_FOG_ATTENUATION_RATE = 0.02f;
    
    float fogWeight = 0.0f;
    if (viewZ >= 75)
    {
        fogWeight += CONSTANT_FOG_SCALE * max(0.0f, 1.0f - exp(-CONSTANT_FOG_ATTENUATION_RATE * viewZ));
    }
    float32_t3 bgColor = gTexture.Sample(gSampler, input.texcoord).rgb;
    float32_t3 fogColor = float32_t3(0.8f, 0.8f, 0.8f);
    float32_t3 color = lerp(bgColor, fogColor, fogWeight);
    output.color = float32_t4(color, 1.0f);
    
    return output;
}