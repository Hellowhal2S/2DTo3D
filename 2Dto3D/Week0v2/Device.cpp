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
	// ���� Ÿ���� �ʱ�ȭ
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
	swapChainDesc.SampleDesc.Count = 1;	// ��Ƽ ���ø� ��Ȱ��ȭ
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ���� Ÿ������ ���
	swapChainDesc.BufferCount = 2; // ���� ���۸�
	swapChainDesc.OutputWindow = hWnd; // �������� â �ڵ�
	swapChainDesc.Windowed = TRUE; // â ���
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // ���� ���

	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
		featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
		&swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
	if (FAILED(result))
	{
		return false;
	}

	m_swapChain->GetDesc(&swapChainDesc); // �ڵ����� ���� �Ǵ� ������ �ٽ� �����;��ϱ� ������ ȣ��

	// ����Ʈ ���� ����
	m_viewport = { 0.0f, 0.0f, (float)swapChainDesc.BufferDesc.Width, (float)swapChainDesc.BufferDesc.Height, 0.0f, 1.0f };
	
	return true;
}

void UDevice::ReleaseDeviceAndSwapChain()
{
	if (m_deviceContext)
	{
		m_deviceContext->Flush(); // �����ִ� GPU ��� ����
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

	// ���� ü�����κ��� �� ���� �ؽ�ó ��������
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_frameBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// ���� Ÿ�� �� ����
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
	renderTargetViewDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB; // ���� ����
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; // 2D �ؽ�ó
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

	//TODO: ���� ����, ���ٽ� ���� ���� �����ϰ� ����Ÿ�ٺ信 �����ϴ� �ڵ带 �߰�

	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, NULL); // ���� Ÿ�� ����

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
	rasterizerDesc.FillMode = D3D11_FILL_SOLID; // ä��� ���
	rasterizerDesc.CullMode = D3D11_CULL_BACK; // �� ���̽� �ø�

	result = m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterState);
	if (FAILED(result))
	{
		return false;
	}
	m_deviceContext->RSSetState(m_rasterState); // �����Ͷ����� ���� ����
	m_deviceContext->RSSetViewports(1, &m_viewport); // GPU�� ȭ���� �������� ���� ����
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
	m_swapChain->Present(1, 0); // VSync Ȱ��ȭ
}

void UDevice::BeginScene()
{
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, ClearColor); // ���� Ÿ�� �� �ʱ�ȭ(���� ������ �����͸� ����)
}

void UDevice::EndScene()
{
	SwapBuffer();
}

