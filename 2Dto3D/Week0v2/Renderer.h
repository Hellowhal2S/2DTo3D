#pragma once

#include "Define.h"

#include "GraphicDevice.h"

class FRenderer {
public:
    FGraphicsDevice* Graphics;
    ID3D11VertexShader* VertexShader = nullptr;
    ID3D11PixelShader* PixelShader = nullptr;
    ID3D11InputLayout* InputLayout = nullptr;
    ID3D11Buffer* ConstantBuffer = nullptr;

    D3D11_VIEWPORT ViewportInfo; // ������ ������ �����ϴ� ����Ʈ ����

    unsigned int Stride;

    void Initialize(FGraphicsDevice* graphics);
    void Release();

    //Init//
    void CreateShader();
    void ReleaseShader();

    void CreateConstantBuffer();
    void ReleaseConstantBuffer();

    ID3D11Buffer* CreateVertexBuffer(FVertexSimple* vertices, UINT byteWidth);
    void ReleaseVertexBuffer(ID3D11Buffer* vertexBuffer);

    //Tick//
    void PrepareShader();

    void UpdateConstant(FMatrix _MVP, float _Flag);

    void RenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices);
};

