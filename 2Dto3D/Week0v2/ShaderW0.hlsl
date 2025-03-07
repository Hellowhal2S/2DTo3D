// ShaderW0.hlsl

// ��� ���� ���� (b0 �������Ϳ� ���ε�)
cbuffer constants : register(b0)
{
    float4x4 World;
    float4x4 View;
    float4x4 Projection;
}

// ���� ���̴� �Է� ����ü
struct VS_INPUT
{
    float4 position : POSITION; // ���� ��ġ
    float4 color : COLOR; // ���� ����
};

// ���� ���̴� ��� & �ȼ� ���̴� �Է� ����ü
struct PS_INPUT
{
    float4 position : SV_POSITION; // ��ȯ�� ��ǥ
    float4 color : COLOR; // ����
};

// ���� ���̴�
PS_INPUT mainVS(VS_INPUT input)
{
    PS_INPUT output;

    // ���� -> �� -> �������� ��ȯ ����
    float4 worldPos = mul(World, input.position);
    float4 viewPos = mul(View, worldPos);
    float4 projPos = mul(Projection, viewPos);

    // ��ȯ�� ��ǥ ����
    output.position = projPos;
    output.color = input.color;
    
    return output;
}

// �ȼ� ���̴�
float4 mainPS(PS_INPUT input) : SV_TARGET
{
    return input.color; // �Էµ� ������ �״�� ���
}
