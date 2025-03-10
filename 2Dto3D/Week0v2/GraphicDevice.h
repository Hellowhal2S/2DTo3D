#pragma once

#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

class FGraphicsDevice {
public:
    ID3D11Device*               Device = nullptr;
    ID3D11DeviceContext*        DeviceContext = nullptr;
    IDXGISwapChain*             SwapChain = nullptr;
    ID3D11Texture2D*            FrameBuffer = nullptr;
    ID3D11RenderTargetView*     FrameBufferRTV = nullptr;
    ID3D11RasterizerState*      RasterizerState = nullptr;

    // Depth-Stencil ���� ����
    ID3D11Texture2D*            DepthStencilBuffer = nullptr;  // ����/���ٽ� �ؽ�ó
    ID3D11DepthStencilView*     DepthStencilView = nullptr;  // ����/���ٽ� ��
    ID3D11DepthStencilState*    DepthStencilState = nullptr;

    FLOAT ClearColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f }; // ȭ���� �ʱ�ȭ(clear) �� �� ����� ����(RGBA)
    
    D3D11_VIEWPORT ViewportInfo;

public:
    void Initialize(HWND hWindow);
    void Release();

    void CreateDeviceAndSwapChain(HWND hWindow);
    void ReleaseDeviceAndSwapChain();
    void CreateFrameBuffer(HWND hWindow);
    void ReleaseFrameBuffer();
    void CreateDepthStencilBuffer(HWND hWindow);
    void ReleaseDepthStencilBuffer();
    void CreateDepthStencilState(HWND hWindow);
	void ReleaseDepthStencilState();
	void CreateRasterizerState(HWND hWindow);
	void ReleaseRasterizerState();
    
    void SwapBuffer();
    
    void Prepare();

    void OnResize(HWND hWindow, float& aspectRatio);
};

