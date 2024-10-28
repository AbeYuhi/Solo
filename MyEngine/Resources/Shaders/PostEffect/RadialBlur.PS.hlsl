#include "FullScreen.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct RadialBlurInfo
{
    int32_t numSamples;
    float32_t blurWidth;
};
ConstantBuffer<RadialBlurInfo> gRadialBlurInfo : register(b0);

struct PixelShaderOutput
{
    float32_t4 color : SV_Target0;
};

PixelShaderOutput main(VertexShaderOutput input)
{    
    const float32_t2 kCenter = float32_t2(0.5f, 0.5f);
    const int32_t kNumSamples = gRadialBlurInfo.numSamples;
    const float32_t kBlurWidth = gRadialBlurInfo.blurWidth;
    //中心から現在のuvに対しての方向を計算
    //普段方向といえば、単位ベクトルだが、ここではあえて正規化せず、遠いほどより遠くをサンプリングする
    float32_t2 direction = input.texcoord - kCenter;
    float32_t3 outputColor = float32_t3(0.0f, 0.0f, 0.0f);
    for (int32_t sampleIndex = 0; sampleIndex < kNumSamples;sampleIndex++){
        //現在のuvから先ほど計算した方向にサンプリング点を進めながらサンプリングしていく
        float32_t2 texcoord = clamp(input.texcoord + direction * kBlurWidth * float32_t(sampleIndex), 0.0f, 1.0f);
        outputColor.rgb += gTexture.Sample(gSampler, texcoord).rgb;
    }
    
    outputColor.rgb *= rcp(kNumSamples);
    
    PixelShaderOutput output;
    output.color.rgb = outputColor;
    output.color.a = 1.0f;
    
    return output;
}