#pragma once

// LINKING //
#pragma comment(lib, "d3d11.lib")

// INCLUDES //
#include <d3d11.h>
#include <d3dcompiler.h>

class UDevice
{
public:
	UDevice();
	UDevice(const UDevice&);
	~UDevice();

public:
    bool Initialize(int, int, HWND);
	void Release();

public:
	bool CreateDeviceAndSwapChain(int, int, HWND hWnd);
	void ReleaseDeviceAndSwapChain();
	//TODO: FrameBuffer�Ӹ��� �ƴ� �ٸ� ���۵鵵 �����ϴ� �Լ��� ������ ��
	bool CreateFrameBuffer();
	void ReleaseFrameBuffer();
	bool CreateRasterizerState();
	void ReleaseRasterizerState();
	void SwapBuffer();

public:
	void BeginScene();
	void EndScene();

public:
    ID3D11Device* GetDevice() const { return m_device; }
	ID3D11DeviceContext* GetDeviceContext() const { return m_deviceContext; }

public:
    FLOAT ClearColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f }; // ȭ���� �ʱ�ȭ(clear)�� �� ����� ���� (RGBA)

private:
    bool m_vsync_enabled;
    int m_videoCardMemory;
    char m_videoCardDescription[128];
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_deviceContext;
    IDXGISwapChain* m_swapChain;
    ID3D11Texture2D* m_frameBuffer;
    ID3D11RenderTargetView* m_renderTargetView;
    ID3D11RasterizerState* m_rasterState;
	D3D11_VIEWPORT m_viewport;
};