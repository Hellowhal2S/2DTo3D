#include "DeviceManager.h"

D3D11_VIEWPORT DeviceManager::ViewportInfo = {};
ID3D11Device* DeviceManager::Device = nullptr;
ID3D11DeviceContext* DeviceManager::DeviceContext = nullptr;
IDXGISwapChain* DeviceManager::SwapChain = nullptr;

void DeviceManager::CreateDeviceAndSwapChain(HWND hWindow) {
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
void DeviceManager::ReleaseDeviceAndSwapChain() {
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