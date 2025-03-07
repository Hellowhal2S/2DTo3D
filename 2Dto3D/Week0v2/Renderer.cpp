#include "Renderer.h"
struct FConstants
{
	FVector Offset;
	float radius;
	float rotationAngle;
};

void URenderer::Create(HWND hWindow)
{
	// 프레임 버퍼 생성
	CreateFrameBuffer();

	// 래스터라이저 상태 생성
	CreateRasterizerState();

}


// 프레임 버퍼를 생성하는 함수
void URenderer::CreateFrameBuffer()
{
	// 스왑 체인으로부터 백 버퍼 텍스처 가져오기
	DeviceManager::SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&FrameBuffer);

	// 렌더 타겟 뷰 생성
	D3D11_RENDER_TARGET_VIEW_DESC framebufferRTVdesc = {};
	framebufferRTVdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB; // 색상 포맷
	framebufferRTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; // 2D 텍스처

	DeviceManager::Device->CreateRenderTargetView(FrameBuffer, &framebufferRTVdesc, &FrameBufferRTV);
}
// 프레임 버퍼를 해제하는 함수
void URenderer::ReleaseFrameBuffer()
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

// 래스터라이저 상태를 생성하는 함수
void URenderer::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterizerdesc = {};
	rasterizerdesc.FillMode = D3D11_FILL_SOLID; // 채우기 모드
	rasterizerdesc.CullMode = D3D11_CULL_BACK; // 백 페이스 컬링

	DeviceManager::Device->CreateRasterizerState(&rasterizerdesc, &RasterizerState);
}
void URenderer::ReleaseRasterizerState()
{
	if (RasterizerState)
	{
		RasterizerState->Release();
		RasterizerState = nullptr;
	}
}

// 렌더러에 사용된 모든 리소스를 해제하는 함수
void URenderer::Release()
{
	RasterizerState->Release();

	// 렌더 타겟을 초기화
	DeviceManager::DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

	ReleaseFrameBuffer();
}

// 스왑 체인의 백 버퍼와 프론트 버퍼를 교체하여 화면에 출력
void URenderer::SwapBuffer()
{
	DeviceManager::SwapChain->Present(1, 0); // 1: VSync 활성화
}

void URenderer::CreateShader()
{
	ID3DBlob* vertexshaderCSO;
	ID3DBlob* pixelshaderCSO;

	D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainVS", "vs_5_0", 0, 0, &vertexshaderCSO, nullptr);

	DeviceManager::Device->CreateVertexShader(vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), nullptr, &SimpleVertexShader);

	D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainPS", "ps_5_0", 0, 0, &pixelshaderCSO, nullptr);

	DeviceManager::Device->CreatePixelShader(pixelshaderCSO->GetBufferPointer(), pixelshaderCSO->GetBufferSize(), nullptr, &SimplePixelShader);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	DeviceManager::Device->CreateInputLayout(layout, ARRAYSIZE(layout), vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), &SimpleInputLayout);

	Stride = sizeof(FVertexSimple);

	vertexshaderCSO->Release();
	pixelshaderCSO->Release();
}

void URenderer::ReleaseShader()
{
	if (SimpleInputLayout)
	{
		SimpleInputLayout->Release();
		SimpleInputLayout = nullptr;
	}

	if (SimplePixelShader)
	{
		SimplePixelShader->Release();
		SimplePixelShader = nullptr;
	}

	if (SimpleVertexShader)
	{
		SimpleVertexShader->Release();
		SimpleVertexShader = nullptr;
	}
}

void URenderer::Prepare()
{
	DeviceManager::DeviceContext->ClearRenderTargetView(FrameBufferRTV, ClearColor); // 렌더 타겟 뷰에 저장된 이전 프레임 데이터를 삭제

	DeviceManager::DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 정정 연결 방식 설정

	DeviceManager::DeviceContext->RSSetViewports(1, &DeviceManager::ViewportInfo); // GPU가 화면을 렌더링할 영역 설정
	DeviceManager::DeviceContext->RSSetState(RasterizerState); //레스터 라이저 상태 설정

	DeviceManager::DeviceContext->OMSetRenderTargets(1, &FrameBufferRTV, nullptr); // 렌더 타겟 설정(백버퍼를 가르킴)
	DeviceManager::DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff); // 블렌뎅 상태 설정, 기본블렌딩 상태임
}

void URenderer::PrepareShader()
{
	DeviceManager::DeviceContext->VSSetShader(SimpleVertexShader, nullptr, 0);
	DeviceManager::DeviceContext->PSSetShader(SimplePixelShader, nullptr, 0);
	DeviceManager::DeviceContext->IASetInputLayout(SimpleInputLayout);

	if (ConstantBuffer)
	{
		DeviceManager::DeviceContext->VSSetConstantBuffers(0, 1, &ConstantBuffer);
	}
}

void URenderer::RenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices)
{
	UINT offset = 0;
	DeviceManager::DeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &Stride, &offset);

	DeviceManager::DeviceContext->Draw(numVertices, 0);
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

	DeviceManager::Device->CreateBuffer(&vertexbufferdesc, &vertexbufferSRD, &vertexBuffer);

	return vertexBuffer;
}

void URenderer::ReleaseVertexBuffer(ID3D11Buffer* vertexBuffer)
{
	vertexBuffer->Release();
}

void URenderer::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC constantbufferdesc = {};
	constantbufferdesc.ByteWidth = sizeof(FConstants) + 0xf & 0xfffffff0; // ensure constant buffer size is multiple of 16 bytes
	constantbufferdesc.Usage = D3D11_USAGE_DYNAMIC; // will be updated from CPU every frame
	constantbufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantbufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	DeviceManager::Device->CreateBuffer(&constantbufferdesc, nullptr, &ConstantBuffer);
}
void URenderer::ReleaseConstantBuffer()
{
	if (ConstantBuffer)
	{
		ConstantBuffer->Release();
		ConstantBuffer = nullptr;
	}
}
void URenderer::UpdateConstant(FVector Offset, float radius, float rotationAngle)
{
	if (ConstantBuffer)
	{
		D3D11_MAPPED_SUBRESOURCE constantbufferMSR;// GPU의 메모리 주소 매핑

		DeviceManager::DeviceContext->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantbufferMSR); // update constant buffer every frame
		FConstants* constants = (FConstants*)constantbufferMSR.pData; //GPU 메모리 직접 접근
		{
			constants->Offset = Offset;
			constants->radius = radius;
			constants->rotationAngle = rotationAngle;
		}
		DeviceManager::DeviceContext->Unmap(ConstantBuffer, 0); // GPU가 다시 사용가능하게 만들기
	}
}