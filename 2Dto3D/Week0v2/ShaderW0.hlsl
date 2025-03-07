// ShaderW0.hlsl

// 상수 버퍼 정의 (b0 레지스터에 바인딩)
cbuffer constants : register(b0)
{
    float4x4 World;
    float4x4 View;
    float4x4 Projection;
}

// 정점 셰이더 입력 구조체
struct VS_INPUT
{
    float4 position : POSITION; // 정점 위치
    float4 color : COLOR; // 정점 색상
};

// 정점 셰이더 출력 & 픽셀 셰이더 입력 구조체
struct PS_INPUT
{
    float4 position : SV_POSITION; // 변환된 좌표
    float4 color : COLOR; // 색상
};

// 정점 셰이더
PS_INPUT mainVS(VS_INPUT input)
{
    PS_INPUT output;

    // 월드 -> 뷰 -> 프로젝션 변환 수행
    float4 worldPos = mul(World, input.position);
    float4 viewPos = mul(View, worldPos);
    float4 projPos = mul(Projection, viewPos);

    // 변환된 좌표 전달
    output.position = projPos;
    output.color = input.color;
    
    return output;
}

// 픽셀 셰이더
float4 mainPS(PS_INPUT input) : SV_TARGET
{
    return input.color; // 입력된 색상을 그대로 출력
}
