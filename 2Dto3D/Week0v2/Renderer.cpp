#include "Renderer.h"

void FRenderer::Initialize(FGraphicsDevice* graphics) {
    Graphics = graphics;
    CreateShader();
    CreateConstantBuffer();
}

void FRenderer::Release() {
    ReleaseShader();
	ReleaseConstantBuffer();
}

void FRenderer::CreateShader() {
    ID3DBlob* vertexshaderCSO;
    ID3DBlob* pixelshaderCSO;

    D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainVS", "vs_5_0", 0, 0, &vertexshaderCSO, nullptr);
    Graphics->Device->CreateVertexShader(vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), nullptr, &VertexShader);

    D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainPS", "ps_5_0", 0, 0, &pixelshaderCSO, nullptr);
    Graphics->Device->CreatePixelShader(pixelshaderCSO->GetBufferPointer(), pixelshaderCSO->GetBufferSize(), nullptr, &PixelShader);

    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    Graphics->Device->CreateInputLayout(layout, ARRAYSIZE(layout), vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), &InputLayout);

    Stride = sizeof(FVertexSimple);
    vertexshaderCSO->Release();
    pixelshaderCSO->Release();
}

void  FRenderer::ReleaseShader()
{
    if (InputLayout)
    {
        InputLayout->Release();
        InputLayout = nullptr;
    }

    if (PixelShader)
    {
        PixelShader->Release();
        PixelShader = nullptr;
    }

    if (VertexShader)
    {
        VertexShader->Release();
        VertexShader = nullptr;
    }
}

void FRenderer::CreateConstantBuffer()
{
    D3D11_BUFFER_DESC constantbufferdesc = {};
    constantbufferdesc.ByteWidth = sizeof(FConstants) + 0xf & 0xfffffff0;
    constantbufferdesc.Usage = D3D11_USAGE_DYNAMIC;
    constantbufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    constantbufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    Graphics->Device->CreateBuffer(&constantbufferdesc, nullptr, &ConstantBuffer);
}

void FRenderer::ReleaseConstantBuffer()
{
    if (ConstantBuffer)
    {
        ConstantBuffer->Release();
        ConstantBuffer = nullptr;
    }
}

ID3D11Buffer* FRenderer::CreateVertexBuffer(FVertexSimple* vertices, UINT byteWidth)
{
    D3D11_BUFFER_DESC vertexbufferdesc = {};
    vertexbufferdesc.ByteWidth = byteWidth;
    vertexbufferdesc.Usage = D3D11_USAGE_IMMUTABLE; // will never be updated 
    vertexbufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexbufferSRD = { vertices };

    ID3D11Buffer* vertexBuffer;

    Graphics->Device->CreateBuffer(&vertexbufferdesc, &vertexbufferSRD, &vertexBuffer);

    return vertexBuffer;
}

void FRenderer::ReleaseVertexBuffer(ID3D11Buffer* vertexBuffer)
{
    vertexBuffer->Release();
}

void FRenderer::PrepareShader()
{
    Graphics->DeviceContext->VSSetShader(VertexShader, nullptr, 0);
    Graphics->DeviceContext->PSSetShader(PixelShader, nullptr, 0);
    Graphics->DeviceContext->IASetInputLayout(InputLayout);

    if (ConstantBuffer)
    {
        Graphics->DeviceContext->VSSetConstantBuffers(0, 1, &ConstantBuffer);
        Graphics->DeviceContext->PSSetConstantBuffers(0, 1, &ConstantBuffer);
    }
}

void FRenderer::UpdateConstant(FMatrix _MVP, float _Flag)
{
    if (ConstantBuffer)
    {
        D3D11_MAPPED_SUBRESOURCE constantbufferMSR;// GPU의 메모리 주소 매핑

        Graphics->DeviceContext->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantbufferMSR); // update constant buffer every frame
        FConstants* constants = (FConstants*)constantbufferMSR.pData; //GPU 메모리 직접 접근
        {
            constants->MVP = _MVP;
            constants->Flag = _Flag;
        }
        Graphics->DeviceContext->Unmap(ConstantBuffer, 0); // GPU가 다시 사용가능하게 만들기
    }
}

void FRenderer::RenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices) {
    UINT offset = 0;
    Graphics->DeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &Stride, &offset);
    Graphics->DeviceContext->Draw(numVertices, 0);
}



