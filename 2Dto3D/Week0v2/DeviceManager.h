#pragma once
#include <Windows.h>

#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

#include <d3d11.h>
#include <d3dcompiler.h>

class DeviceManager {
public:
	static D3D11_VIEWPORT ViewportInfo;
	static ID3D11Device* Device;
	static ID3D11DeviceContext* DeviceContext;
	static IDXGISwapChain* SwapChain;

	//DeviceManager();
	//~DeviceManager();

	static void CreateDeviceAndSwapChain(HWND hWindow);
	static void ReleaseDeviceAndSwapChain();
};