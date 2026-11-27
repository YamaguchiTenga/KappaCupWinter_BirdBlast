// Dedicated, unlit additive material. UV.x identifies the model layer.
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};
cbuffer BeamCb : register(b1)
{
    float4 beam; // elapsed seconds, reserved, reserved, reserved
    float4 eyePosition;
};
struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};
struct PSInput
{
    float4 position : SV_POSITION;
    float3 worldPosition : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float2 layer : TEXCOORD2;
    float localZ : TEXCOORD3;
};
PSInput VSMain(VSInput input)
{
    PSInput output;
    float3 p = input.position;
    float tag = input.uv.x;
    float time = beam.x;
    float pulse = 1.0 + 0.025 * sin(time * 50.0 + p.z * 0.018);
    if (tag < 3.0)
        p.xy *= pulse;
    if (tag > 4.5)
    {
        float travel = frac(input.uv.y + time * 2.0);
        p.z += (travel - input.uv.y) * 1000.0;
        if (tag < 5.5)
            p.xy *= 0.78 + travel * 0.35;
    }
    float4 world = mul(mWorld, float4(p, 1.0));
    output.position = mul(mProj, mul(mView, world));
    output.worldPosition = world.xyz;
    output.normal = mul((float3x3)mWorld, input.normal);
    output.layer = input.uv;
    output.localZ = p.z;
    return output;
}
float4 PSMain(PSInput input) : SV_Target0
{
    float tag = input.layer.x;
    float3 colour;
    float strength = 1.0;
    if (tag < 0.5)
        colour = float3(1.8, 1.8, 1.8);
    else if (tag < 1.5)
        colour = float3(0.009, 0.15, 0.39);
    else if (tag < 2.5)
        colour = float3(0.0, 0.0175, 0.05);
    else if (tag < 4.5)
    {
        float3 viewDirection = normalize(eyePosition.xyz - input.worldPosition);
        float facing = abs(dot(normalize(input.normal), viewDirection));
        strength = 0.25 + 0.75 * pow(facing, 2.0);
        colour = float3(0.06, 0.325, 0.6);
    }
    else if (tag < 5.5)
    {
        colour = float3(0.6, 0.84, 0.96);
        float travel = frac(input.layer.y + beam.x * 2.0);
        strength = smoothstep(0.0, 0.08, travel) * (1.0 - smoothstep(0.85, 1.0, travel));
    }
    else
    {
        colour = float3(0.075, 0.275, 0.45);
        strength = saturate(input.localZ / 60.0) * saturate((1000.0 - input.localZ) / 80.0);
    }
    if (tag > 0.5 && tag < 2.5)
    {
        float3 viewDirection = normalize(eyePosition.xyz - input.worldPosition);
        float facing = abs(dot(normalize(input.normal), viewDirection));
        strength *= pow(saturate(facing), 0.7);
    }
    return float4(colour * strength, 0.0);
}
