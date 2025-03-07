#include "DeviceManager.h"

D3D11_VIEWPORT DeviceManager::ViewportInfo = {};
ID3D11Device* DeviceManager::Device = nullptr;
ID3D11DeviceContext* DeviceManager::DeviceContext = nullptr;
IDXGISwapChain* DeviceManager::SwapChain = nullptr;

void DeviceManager::CreateDeviceAndSwapChain(HWND hWindow) {
	// 지원하는 Direct3D 기능 레벨을 정의
	D3D_FEATURE_LEVEL featurelevels[] = { D3D_FEATURE_LEVEL_11_0 };

	// 스왑 체인 설정 구조체 초기화
	DXGI_SWAP_CHAIN_DESC swapchaindesc = {};
	swapchaindesc.BufferDesc.Width = 0; // 창 크기에 맞게 자동으로 설정
	swapchaindesc.BufferDesc.Height = 0; // 창 크기에 맞게 자동으로 설정
	swapchaindesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // 색상 포맷
	swapchaindesc.SampleDesc.Count = 1; // 멀티 샘플링 비활성화
	swapchaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 렌더 타겟으로 사용
	swapchaindesc.BufferCount = 2; // 더블 버퍼링
	swapchaindesc.OutputWindow = hWindow; // 렌더링할 창 핸들
	swapchaindesc.Windowed = TRUE; // 창 모드
	swapchaindesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // 스왑 방식

	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
		featurelevels, ARRAYSIZE(featurelevels), D3D11_SDK_VERSION,
		&swapchaindesc, &SwapChain, &Device, nullptr, &DeviceContext);

	SwapChain->GetDesc(&swapchaindesc);// 자동으로 설정 되는 값들을 다시 가져와야하기 때문에 호출

	// 뷰포트 정보 설정
	ViewportInfo = { 0.0f, 0.0f, (float)swapchaindesc.BufferDesc.Width, (float)swapchaindesc.BufferDesc.Height, 0.0f, 1.0f };
}
void DeviceManager::ReleaseDeviceAndSwapChain() {
	if (DeviceContext)
	{
		DeviceContext->Flush(); // 남아있는 GPU 명령 실행
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