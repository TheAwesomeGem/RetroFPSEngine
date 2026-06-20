// =============== Structs ===============
struct GlobalData
{
    matrix pos_transform;
    matrix normal_transform;
    float4 tint_color;
};
struct VsIn
{
    float3 pos : Position;
    float3 normal: Normal;
};
struct VsOut
{
    float4 pos : SV_Position;
    float3 normal: Normal;
};
// =======================================

// =============== Variables ===============
cbuffer GlobalDataCBuffer : register(b0)
{
    GlobalData g_global;
};
// =========================================

VsOut main(VsIn input)
{
    VsOut output;
    output.pos = mul(float4(input.pos, 1.0f), g_global.pos_transform);
    output.normal = normalize(mul(float4(input.normal, 0.0f), g_global.normal_transform).xyz);
    return output;
}
