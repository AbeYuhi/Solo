#include "FullScreen.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
	float32_t4 color : SV_Target0;
};

struct VignetteBlurInfo
{
    float intensity; // 赤みの強度調整用
    float blurAmount; // ぼかしの強度調整用
};
ConstantBuffer<VignetteBlurInfo> gVignetteBlurInfo : register(b0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSampler, input.texcoord);
    
    uint32_t width, height;
    gTexture.GetDimensions(width, height);
    // 正規化されたUV座標
    float2 normalizedUV = input.position.xy / float2(width, height);
    
    // 中心からの距離を計算
    float2 center = float2(0.5f, 0.5f);
    float dist = distance(normalizedUV, center);

    // 赤みの強度を距離に基づいて計算
    float redIntensity = smoothstep(0.4f, 0.8f, dist) * gVignetteBlurInfo.intensity;

    // 簡易的なぼかし処理
    float4 blurredColor = gTexture.Sample(gSampler, input.texcoord);
    
    // 距離に応じたぼかしの適用
    if (dist > 0.4)
    {
        //// サンプル位置のオフセットを指定
        //float2 offset = gVignetteBlurInfo.blurAmount * (dist - 0.4f) * float2(1.0f / width, 1.0f / height);
        //blurredColor += gTexture.Sample(gSampler, input.texcoord + offset);
        //blurredColor += gTexture.Sample(gSampler, input.texcoord - offset);
        //blurredColor /= 3; // 平均化してぼかし効果を出す
        
        float2 offset1 = blurAmount * float2(1.0 / resolution.x, 0.0);
        float2 offset2 = blurAmount * float2(0.0, 1.0 / resolution.y);
        float2 offset3 = blurAmount * float2(1.0 / resolution.x, 1.0 / resolution.y);
        float2 offset4 = blurAmount * float2(-1.0 / resolution.x, -1.0 / resolution.y);

        // オフセットした位置からのサンプリング
        blurredColor += gTexture.Sample(gSampler, input.texcoord + offset1);
        blurredColor += gTexture.Sample(gSampler, input.texcoord - offset1);
        blurredColor += gTexture.Sample(gSampler, input.texcoord + offset2);
        blurredColor += gTexture.Sample(gSampler, input.texcoord - offset2);
        blurredColor += gTexture.Sample(gSampler, input.texcoord + offset3);
        blurredColor += gTexture.Sample(gSampler, input.texcoord - offset3);
        blurredColor += gTexture.Sample(gSampler, input.texcoord + offset4);
        blurredColor += gTexture.Sample(gSampler, input.texcoord - offset4);
        
        blurredColor /= 9; // 平均化してぼかし効果を出す
    }

    // 赤みとぼかしを適用して最終色を計算
    output.color = lerp(blurredColor, float4(1.0, 0.0, 0.0, 1.0), redIntensity);
    
    return output;
}