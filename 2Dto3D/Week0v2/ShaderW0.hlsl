// ShaderW0.hlsl
cbuffer constants : register(b0)
{
    float3 Offset;
    float radius;
    float rotationAngle;
}

float4 Rotate(float4 pos, float4 angle)
{
    float cosA = cos(angle);
    float sinA = sin(angle);
    return float4(
        pos.x * cosA - pos.y * sinA,
        pos.x * sinA + pos.y * cosA,
        0.f,
        1.0f
    );
}

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
    PS_INPUT output;
    
    // Pass the position directly to the pixel shader (no transformation)
    float4 scaledPos = input.position * float4(radius, radius, radius, 1.0);
    float4 rotatePos = Rotate(scaledPos, rotationAngle);

    output.position = float4(Offset, 0) + rotatePos;
    //output.position = float4(Offset, 0) + (input.position * radius);
    
    // Pass the color to the pixel shader
    if(rotationAngle ==0.0f)
        output.color = float4(0.0, 0.0, 0.0, 0.0);
    else
        output.color = input.color;
    
    return output;
}

float4 mainPS(PS_INPUT input) : SV_TARGET
{
    // Output the color directly
    return input.color;
}