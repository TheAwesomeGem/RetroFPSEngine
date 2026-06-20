// =============== Structs ===============
struct GlobalData
{
    matrix pos_transform;
    matrix normal_transform;
    float4 tint_color;
};
struct PsIn
{
    uint t_id: SV_PrimitiveID;
    float3 normal: Normal;
};
// =======================================

// =============== Variables ===============
static const float4 AMBIENT_LIGHT_COLOR = float4(0.05F, 0.05F, 0.05F, 1.0F);
static const float4 DIRECTIONAL_LIGHT_COLOR = float4(1.05F, 1.02F, 0.96F, 1.0F);
static const float3 DIRECTIONAL_LIGHT_DIR = float3(0.0F, -0.707F, 0.707F);
cbuffer GlobalDataCBuffer : register(b0)
{
    GlobalData g_global;
};
Texture2D crate_tex : register(t0);
SamplerState basic_sampler: register(s0);
// =========================================

float4 main(PsIn input): SV_Target
{
    float light_intensity = max(dot(-DIRECTIONAL_LIGHT_DIR, normalize(input.normal)), 0.0F);
    float4 diffuse_color = g_global.tint_color;
    float4 directional_color = DIRECTIONAL_LIGHT_COLOR * light_intensity;
    float4 lit_color = clamp(diffuse_color * (directional_color + AMBIENT_LIGHT_COLOR), 0.0f, 1.0f);

    return lit_color;
}
