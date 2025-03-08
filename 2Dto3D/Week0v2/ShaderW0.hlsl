// ShaderW0.hlsl
cbuffer constants : register(b0)
{
    row_major float4x4 MVP;
}

//float4 Rotate(float4 pos, float4 angle)
//{
//    float cosA = cos(angle);
//    float sinA = sin(angle);
//    return float4(
//        pos.x * cosA - pos.y * sinA,
//        pos.x * sinA + pos.y * cosA,
//        0.f,
//        1.0f
//    );
//}

struct VS_INPUT
{
    float4 position : POSITION; // Input position from vertex buffer
    float4 color : COLOR; // Input color from vertex buffer
};

struct PS_INPUT
{
    float4 position : SV_POSITION; // Transformed position to pass to the pixel shader
    float4 color : COLOR; // Color to pass to the pixel shader
};


PS_INPUT mainVS(VS_INPUT input)
{
    PS_INPUT output = input;
    
    float4x4 MVProjection = MVP;
    output.position = mul(float4(input.position. xyz, 1.0f), MVProjection);
    output.color = input.color;
    return output;
}

float4 mainPS(PS_INPUT input) : SV_TARGET
{
    // Output the color directly
    return input.color;
}