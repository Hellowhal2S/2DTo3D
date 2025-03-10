// 상수 버퍼 정의 (b0 레지스터에 바인딩)
cbuffer constants : register(b0)
{
    float4x4 Mat;
    int mode; // 모드 추가
    int padding1;
    int padding2;
    int padding3;  //16바이트 포맷을 맞추기 위한 패딩들...
    
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
 
    float4 projPos = mul(Mat, input.position);

    // 변환된 좌표 전달
    output.position = projPos;
    output.color = input.color;
    
    return output;
}

// 픽셀 셰이더 (mode 값에 따라 색상 변경)
float4 mainPS(PS_INPUT input) : SV_TARGET
{
    float4 baseColor = input.color; // 기본 색상

    if (mode == 1)
    {
        baseColor = float4(1, 0, 0, 1); // 빨강
    }
    else if (mode == 2)
    {
        baseColor = float4(0, 1, 0, 1); // 초록
    }
    else if (mode == 3)
    {
        baseColor = float4(0, 0, 1, 1); // 파랑
    }
    else if (mode == 4)
    {
        baseColor *= 2.0f; // 밝게 (강조 효과)
    }

    return baseColor;
}
