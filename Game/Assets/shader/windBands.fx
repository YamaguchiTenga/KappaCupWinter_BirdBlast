// AI実装：風の帯を回転させる専用シェーダー。発光は加算しない。
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};
cbuffer WindCb : register(b1)
{
    float4 wind; // x: elapsed seconds, y: visibility
};
struct VSInput
{
    float3 position : POSITION;
    float2 uv : TEXCOORD0;
};
struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};
PSInput VSMain(VSInput input)
{
    PSInput output;
    float3 p = input.position;
    float layer = floor(input.uv.x);
    if (layer < 3.0)
    {
        float angle = wind.x * (4.5 + layer * 0.7);
        float s = sin(angle), c = cos(angle);
        p.xz = float2(p.x * c - p.z * s, p.x * s + p.z * c);
        p.y += sin(wind.x * 4.0 + layer * 2.0) * 3.0;
    }
    else
    {
        // 尾は後方へ伸びたまま、わずかに波打たせる。
        float along = frac(input.uv.x);
        p.x += sin(wind.x * 12.0 - along * 7.0 + layer) * along * 4.0;
    }
    output.position = mul(mProj, mul(mView, mul(mWorld, float4(p, 1.0))));
    output.uv = input.uv;
    return output;
}
float4 PSMain(PSInput input) : SV_Target0
{
    float along = frac(input.uv.x);
    float across = input.uv.y;
    float edge = smoothstep(0.0, 0.25, across) * (1.0 - smoothstep(0.75, 1.0, across));
    float ends = smoothstep(0.0, 0.13, along) * (1.0 - smoothstep(0.65, 1.0, along));
    float centre = 1.0 - abs(across * 2.0 - 1.0);
    float3 colour = lerp(float3(0.40, 0.76, 0.82), float3(0.78, 0.91, 0.93), centre);
    float opacity = 0.36 * edge * ends * wind.y;
    if (input.uv.x >= 3.0)
    {
        opacity *= 0.7;
    }
    return float4(colour, opacity);
}
