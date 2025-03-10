#pragma once
#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

#include <d3d11.h>
#include <d3dcompiler.h>
#include "Define.h"
#include "Matrix.h" 

class UGraphicsDevice;
class URenderer {
public:
    UGraphicsDevice* Graphics;
    ID3D11VertexShader* VertexShader = nullptr;
    ID3D11PixelShader* PixelShader = nullptr;
    ID3D11InputLayout* InputLayout = nullptr;
    ID3D11Buffer* ConstantBuffer = nullptr;

    ID3D11Texture2D* DepthStencilBuffer = nullptr;
    ID3D11DepthStencilView* DepthStencilView = nullptr;
    ID3D11DepthStencilState* DepthStencilState = nullptr;

    D3D11_VIEWPORT ViewportInfo;
    unsigned int Stride;

    void Initialize(UGraphicsDevice* graphics);
    void Release();
    void Prepare();
    void RenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices);
    void CreateShader();
    void CreateConstantBuffer();
    void ReleaseShader();
    void PrepareShader();
    ID3D11Buffer* CreateVertexBuffer(FVertexSimple* vertices, UINT byteWidth);
    void ReleaseVertexBuffer(ID3D11Buffer* vertexBuffer);
    void ReleaseConstantBuffer();
    void UpdateConstant(const FMatrix& Mat, int mode);
    void CreateDepthBuffer();
    void ReleaseDepthBuffer();

    struct FConstants
    {
        FMatrix Mat;
        int mode;
        int padding1;
        int padding2;
        int padding3;
    };
};


