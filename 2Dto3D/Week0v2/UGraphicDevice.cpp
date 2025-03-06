#include "UGraphicDevice.h"

void UGraphicsDevice::Initialize(HWND hWindow) {
    CreateDeviceAndSwapChain(hWindow);
    CreateFrameBuffer();
    D3D11_RASTERIZER_DESC rasterizerdesc = {};
    rasterizerdesc.FillMode = D3D11_FILL_SOLID;
    rasterizerdesc.CullMode = D3D11_CULL_BACK;
    Device->CreateRasterizerState(&rasterizerdesc, &RasterizerState);
}

void UGraphicsDevice::CreateDeviceAndSwapChain(HWND hWindow)
{
    // �����ϴ� Direct3D ��� ������ ����
    D3D_FEATURE_LEVEL featurelevels[] = { D3D_FEATURE_LEVEL_11_0 };

    // ���� ü�� ���� ����ü �ʱ�ȭ
    DXGI_SWAP_CHAIN_DESC swapchaindesc = {};
    swapchaindesc.BufferDesc.Width = 0; // â ũ�⿡ �°� �ڵ����� ����
    swapchaindesc.BufferDesc.Height = 0; // â ũ�⿡ �°� �ڵ����� ����
    swapchaindesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // ���� ����
    swapchaindesc.SampleDesc.Count = 1; // ��Ƽ ���ø� ��Ȱ��ȭ
    swapchaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ���� Ÿ������ ���
    swapchaindesc.BufferCount = 2; // ���� ���۸�
    swapchaindesc.OutputWindow = hWindow; // �������� â �ڵ�
    swapchaindesc.Windowed = TRUE; // â ���
    swapchaindesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // ���� ���

    D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
        featurelevels, ARRAYSIZE(featurelevels), D3D11_SDK_VERSION,
        &swapchaindesc, &SwapChain, &Device, nullptr, &DeviceContext);

    SwapChain->GetDesc(&swapchaindesc);// �ڵ����� ���� �Ǵ� ������ �ٽ� �����;��ϱ� ������ ȣ��

    // ����Ʈ ���� ����
    ViewportInfo = { 0.0f, 0.0f, (float)swapchaindesc.BufferDesc.Width, (float)swapchaindesc.BufferDesc.Height, 0.0f, 1.0f };
}

void UGraphicsDevice::ReleaseDeviceAndSwapChain()
{
    if (DeviceContext)
    {
        DeviceContext->Flush(); // �����ִ� GPU ��� ����
    }

    if (SwapChain)
    {
        SwapChain->Release();
        SwapChain = nullptr;
    }

    if (Device)
    {
        Device->Release();
        Device = nullptr;
    }

    if (DeviceContext)
    {
        DeviceContext->Release();
        DeviceContext = nullptr;
    }
}

void UGraphicsDevice::CreateFrameBuffer()
{
    // ���� ü�����κ��� �� ���� �ؽ�ó ��������
    SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&FrameBuffer);

    // ���� Ÿ�� �� ����
    D3D11_RENDER_TARGET_VIEW_DESC framebufferRTVdesc = {};
    framebufferRTVdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB; // ���� ����
    framebufferRTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; // 2D �ؽ�ó

    Device->CreateRenderTargetView(FrameBuffer, &framebufferRTVdesc, &FrameBufferRTV);
}

void UGraphicsDevice::ReleaseFrameBuffer()
{
    if (FrameBuffer)
    {
        FrameBuffer->Release();
        FrameBuffer = nullptr;
    }

    if (FrameBufferRTV)
    {
        FrameBufferRTV->Release();
        FrameBufferRTV = nullptr;
    }
}

void UGraphicsDevice::ReleaseRasterizerState()
{
    if (RasterizerState)
    {
        RasterizerState->Release();
        RasterizerState = nullptr;
    }
}

void UGraphicsDevice::Release() {
    RasterizerState->Release();

    DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

    ReleaseFrameBuffer();
    ReleaseDeviceAndSwapChain();
}

void UGraphicsDevice::SwapBuffer() {
    SwapChain->Present(1, 0);
}
void UGraphicsDevice::Prepare()
{
    DeviceContext->ClearRenderTargetView(FrameBufferRTV, ClearColor); // ���� Ÿ�� �信 ����� ���� ������ �����͸� ����

    DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // ���� ���� ��� ����

    DeviceContext->RSSetViewports(1, &ViewportInfo); // GPU�� ȭ���� �������� ���� ����
    DeviceContext->RSSetState(RasterizerState); //������ ������ ���� ����

    DeviceContext->OMSetRenderTargets(1, &FrameBufferRTV, nullptr); // ���� Ÿ�� ����(����۸� ����Ŵ)
    DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff); // ���� ���� ����, �⺻���� ������
}