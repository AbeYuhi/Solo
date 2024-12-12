#include "FullScreen.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
Texture2D<float32_t4> gDepthTexture : register(t1);
SamplerState gSampler : register(s0);
SamplerState gDepthSampler : register(s1);

struct PixelShaderOutput
{
    float32_t4 color : SV_Target0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float32_t4 color;
    color.rgb = float32_t3(0.0f, 0.0f, 0.0f);
    color.a = 1.0f;
    
    //フォグカラー
    float32_t4 fogColor;
    fogColor.rgb = float32_t3(1.0f, 1.0f, 1.0f);
    fogColor.a = 1.0f;
    
    // 深度値を取得
    float depth = gDepthTexture.Sample(gDepthSampler, input.texcoord);
    // 霧の濃度を計算
    float fogFactor = saturate((depth - 0.2f) / (1.0f - 0.25f));

    // 元の色と霧の色をブレンド
    color = gTexture.Sample(gSampler, input.texcoord);
    color = lerp(color, fogColor, fogFactor);
    output.color = color;
    return output;
}