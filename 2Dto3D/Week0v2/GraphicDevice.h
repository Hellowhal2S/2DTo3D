#pragma once
#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

#include <windows.h>
#include <d3d11.h>

class UGraphicsDevice {
public:
    ID3D11Device* Device = nullptr;
    ID3D11DeviceContext* DeviceContext = nullptr;
    IDXGISwapChain* SwapChain = nullptr;
    ID3D11Texture2D* FrameBuffer = nullptr;
    ID3D11RenderTargetView* FrameBufferRTV = nullptr;
    ID3D11RasterizerState* RasterizerState = nullptr;

    FLOAT ClearColor[4] = { 0.025f, 0.025f, 0.025, 1.0f }; // ȭ���� �ʱ�ȭ(clear) �� �� ����� ����(RGBA)
    D3D11_VIEWPORT ViewportInfo;


    void Initialize(HWND hWindow);
    void CreateDeviceAndSwapChain(HWND hWindow);
    void ReleaseDeviceAndSwapChain();
    void CreateFrameBuffer();
    void ReleaseFrameBuffer();
    void ReleaseRasterizerState();
    void Release();
    void SwapBuffer();
    void Prepare();
};

