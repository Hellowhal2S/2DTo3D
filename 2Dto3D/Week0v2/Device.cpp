#include "Device.h"

UDevice::UDevice()
{
	m_device = NULL;
	m_deviceContext = NULL;
	m_swapChain = NULL;
	m_frameBuffer = NULL;
	m_renderTargetView = NULL;
	m_rasterState = NULL;
}

UDevice::UDevice(const UDevice& other)
{
}

UDevice::~UDevice()
{
}

bool UDevice::Initialize(int screenWidth, int screenHeight, HWND hWnd)
{
	bool bResult;
	bResult = CreateDeviceAndSwapChain(screenWidth, screenHeight, hWnd);
	if (!bResult)
	{
		return false;
	}
	bResult = CreateFrameBuffer();
	if (!bResult)
	{
		return false;
	}
	bResult = CreateRasterizerState();
	if (!bResult)
	{
		return false;
	}
	return true;
}

void UDevice::Release()
{
	ReleaseRasterizerState();
	// 렌더 타겟을 초기화
	m_deviceContext->OMSetRenderTargets(0, NULL, NULL);
	ReleaseFrameBuffer();
	ReleaseDeviceAndSwapChain();
}

bool UDevice::CreateDeviceAndSwapChain(int screenWidth, int screenHeight, HWND hWnd)
{
	HRESULT result;

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;	// 멀티 샘플링 비활성화
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 렌더 타겟으로 사용
	swapChainDesc.BufferCount = 2; // 더블 버퍼링
	swapChainDesc.OutputWindow = hWnd; // 렌더링할 창 핸들
	swapChainDesc.Windowed = TRUE; // 창 모드
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // 스왑 방식

	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
		featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
		&swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
	if (FAILED(result))
	{
		return false;
	}

	m_swapChain->GetDesc(&swapChainDesc); // 자동으로 설정 되는 값들을 다시 가져와야하기 때문에 호출

	// 뷰포트 정보 설정
	m_viewport = { 0.0f, 0.0f, (float)swapChainDesc.BufferDesc.Width, (float)swapChainDesc.BufferDesc.Height, 0.0f, 1.0f };
	
	return true;
}

void UDevice::ReleaseDeviceAndSwapChain()
{
	if (m_deviceContext)
	{
		m_deviceContext->Flush(); // 남아있는 GPU 명령 실행
	}
	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = NULL;
	}
	if (m_device)
	{
		m_device->Release();
		m_device = NULL;
	}
	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = NULL;
	}
}

bool UDevice::CreateFrameBuffer()
{
	HRESULT result;

	// 스왑 체인으로부터 백 버퍼 텍스처 가져오기
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_frameBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// 렌더 타겟 뷰 생성
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
	renderTargetViewDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB; // 색상 포맷
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; // 2D 텍스처
	result = m_device->CreateRenderTargetView(m_frameBuffer, &renderTargetViewDesc, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	//TODO: Check if this is necessary
	/*
	m_frameBuffer->Release();
	m_frameBuffer = NULL;
	*/

	//TODO: 깊이 버퍼, 스텐실 버퍼 등을 생성하고 렌더타겟뷰에 세팅하는 코드를 추가

	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, NULL); // 렌더 타겟 설정

	return true;
}

void UDevice::ReleaseFrameBuffer()
{
	if (m_frameBuffer)
	{
		m_frameBuffer->Release();
		m_frameBuffer = NULL;
	}
	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = NULL;
	}
}

bool UDevice::CreateRasterizerState()
{
	HRESULT result;

	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID; // 채우기 모드
	rasterizerDesc.CullMode = D3D11_CULL_BACK; // 백 페이스 컬링

	result = m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterState);
	if (FAILED(result))
	{
		return false;
	}
	m_deviceContext->RSSetState(m_rasterState); // 래스터라이저 상태 설정
	m_deviceContext->RSSetViewports(1, &m_viewport); // GPU가 화면을 렌더링할 영역 설정
	return true;
}

void UDevice::ReleaseRasterizerState()
{
	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = NULL;
	}
}

void UDevice::SwapBuffer()
{
	m_swapChain->Present(1, 0); // VSync 활성화
}

void UDevice::BeginScene()
{
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, ClearColor); // 렌더 타겟 뷰 초기화(이전 프레임 데이터를 삭제)
}

void UDevice::EndScene()
{
	SwapBuffer();
}

