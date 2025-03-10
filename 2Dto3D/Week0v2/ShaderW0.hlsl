// ��� ���� ���� (b0 �������Ϳ� ���ε�)
cbuffer constants : register(b0)
{
    float4x4 Mat;
    int mode; // ��� �߰�
    int padding1;
    int padding2;
    int padding3;  //16����Ʈ ������ ���߱� ���� �е���...
    
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
 
    float4 projPos = mul(Mat, input.position);

    // ��ȯ�� ��ǥ ����
    output.position = projPos;
    output.color = input.color;
    
    return output;
}

// �ȼ� ���̴� (mode ���� ���� ���� ����)
float4 mainPS(PS_INPUT input) : SV_TARGET
{
    float4 baseColor = input.color; // �⺻ ����

    if (mode == 1)
    {
        baseColor = float4(1, 0, 0, 1); // ����
    }
    else if (mode == 2)
    {
        baseColor = float4(0, 1, 0, 1); // �ʷ�
    }
    else if (mode == 3)
    {
        baseColor = float4(0, 0, 1, 1); // �Ķ�
    }
    else if (mode == 4)
    {
        baseColor *= 2.0f; // ��� (���� ȿ��)
    }

    return baseColor;
}
