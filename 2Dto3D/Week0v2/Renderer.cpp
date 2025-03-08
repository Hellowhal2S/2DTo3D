#include "Renderer.h"
#include "GraphicDevice.h"
void URenderer::Initialize(UGraphicsDevice* graphics) {
    Graphics = graphics;
    CreateShader();
    CreateConstantBuffer();
    CreateDepthBuffer();  // ±íÀÌ ¹öÆÛ »ý¼º Ãß°¡
}


void URenderer::Release() {
    ReleaseShader();
    ReleaseConstantBuffer();
    ReleaseDepthBuffer();  // ±íÀÌ ¹öÆÛ ÇØÁ¦ Ãß°¡
}


void URenderer::Prepare()
{
    float ClearColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f };
    Graphics->DeviceContext->ClearRenderTargetView(Graphics->FrameBufferRTV, ClearColor);
    Graphics->DeviceContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    Graphics->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    Graphics->DeviceContext->RSSetViewports(1, &Graphics->ViewportInfo);
    Graphics->DeviceContext->RSSetState(Graphics->RasterizerState);
    Graphics->DeviceContext->OMSetRenderTargets(1, &Graphics->FrameBufferRTV, DepthStencilView);
    Graphics->DeviceContext->OMSetDepthStencilState(DepthStencilState, 1);
}



void URenderer::CreateShader() {
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
void  URenderer::ReleaseShader()
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
void URenderer::PrepareShader()
{
    Graphics->DeviceContext->VSSetShader(VertexShader, nullptr, 0);
    Graphics->DeviceContext->PSSetShader(PixelShader, nullptr, 0);
    Graphics->DeviceContext->IASetInputLayout(InputLayout);

    if (ConstantBuffer)
    {
        Graphics->DeviceContext->VSSetConstantBuffers(0, 1, &ConstantBuffer);
    }
}
void URenderer::RenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices) {
    UINT offset = 0;
    Graphics->DeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &Stride, &offset);
    Graphics->DeviceContext->Draw(numVertices, 0);
}

ID3D11Buffer* URenderer::CreateVertexBuffer(FVertexSimple* vertices, UINT byteWidth)
{
    // 2. Create a vertex buffer
    D3D11_BUFFER_DESC vertexbufferdesc = {};
    vertexbufferdesc.ByteWidth = byteWidth;
    vertexbufferdesc.Usage = D3D11_USAGE_IMMUTABLE; // will never be updated 
    vertexbufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexbufferSRD = { vertices };

    ID3D11Buffer* vertexBuffer;

    Graphics->Device->CreateBuffer(&vertexbufferdesc, &vertexbufferSRD, &vertexBuffer);

    return vertexBuffer;
}

void URenderer::ReleaseVertexBuffer(ID3D11Buffer* vertexBuffer)
{
    vertexBuffer->Release();
}

void URenderer::CreateConstantBuffer()
{
    D3D11_BUFFER_DESC constantbufferdesc = {};
    constantbufferdesc.ByteWidth = sizeof(FConstants);
    constantbufferdesc.Usage = D3D11_USAGE_DYNAMIC;
    constantbufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    constantbufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    Graphics->Device->CreateBuffer(&constantbufferdesc, nullptr, &ConstantBuffer);
}


void URenderer::ReleaseConstantBuffer()
{
    if (ConstantBuffer)
    {
        ConstantBuffer->Release();
        ConstantBuffer = nullptr;
    }
}
void URenderer::UpdateConstant(const FMatrix& worldMatrix, const FMatrix& viewMatrix, const FMatrix& projectionMatrix)
{
    if (ConstantBuffer)
    {
        D3D11_MAPPED_SUBRESOURCE constantbufferMSR;

        Graphics->DeviceContext->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantbufferMSR);
        FConstants* constants = (FConstants*)constantbufferMSR.pData;
        {
            constants->World = worldMatrix;
            constants->View = viewMatrix;
            constants->Projection = projectionMatrix;
        }
        Graphics->DeviceContext->Unmap(ConstantBuffer, 0);
    }
}

void URenderer::CreateDepthBuffer()
{
    // ±íÀÌ ½ºÅÙ½Ç ÅØ½ºÃ³ »ý¼º
    D3D11_TEXTURE2D_DESC depthBufferDesc = {};
    depthBufferDesc.Width = (UINT)Graphics->ViewportInfo.Width;
    depthBufferDesc.Height = (UINT)Graphics->ViewportInfo.Height;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    Graphics->Device->CreateTexture2D(&depthBufferDesc, nullptr, &DepthStencilBuffer);

    // ±íÀÌ ½ºÅÙ½Ç ºä »ý¼º
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
    depthStencilViewDesc.Format = depthBufferDesc.Format;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    Graphics->Device->CreateDepthStencilView(DepthStencilBuffer, &depthStencilViewDesc, &DepthStencilView);

    // ±íÀÌ ½ºÅÙ½Ç »óÅÂ »ý¼º
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    Graphics->Device->CreateDepthStencilState(&depthStencilDesc, &DepthStencilState);
}

void URenderer::ReleaseDepthBuffer()
{
    if (DepthStencilBuffer) {
        DepthStencilBuffer->Release();
        DepthStencilBuffer = nullptr;
    }
    if (DepthStencilView) {
        DepthStencilView->Release();
        DepthStencilView = nullptr;
    }
    if (DepthStencilState) {
        DepthStencilState->Release();
        DepthStencilState = nullptr;
    }
}
