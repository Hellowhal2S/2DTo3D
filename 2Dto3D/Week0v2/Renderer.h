#pragma once
#include <Windows.h>

#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

#include <d3d11.h>
#include <d3dcompiler.h>

#include "VectorUtils.h"
#include "Vertex.h"
#include "DeviceManager.h"

class URenderer
{

public:
	// 렌더링에 필요한 리소스 및 상태를 관리하기 위한 변수들
	ID3D11Texture2D* FrameBuffer = nullptr; // 화면 출력용 텍스처
	ID3D11RenderTargetView* FrameBufferRTV = nullptr; // 텍스처를 렌더 타겟으로 사용하는 뷰
	ID3D11RasterizerState* RasterizerState = nullptr; // 래스터라이저 상태(컬링, 채우기 모드 등 정의)
	ID3D11Buffer* ConstantBuffer = nullptr; // 쉐이더에 데이터를 전달하기 위한 상수 버퍼

	FLOAT ClearColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f }; // 화면을 초기화(clear)할 때 사용할 색상 (RGBA)

	ID3D11VertexShader* SimpleVertexShader;
	ID3D11PixelShader* SimplePixelShader;
	ID3D11InputLayout* SimpleInputLayout;
	unsigned int Stride;
public:

	// 렌더러 초기화 함수
	void Create(HWND hWindow);
	// 프레임 버퍼를 생성하는 함수
	void CreateFrameBuffer();
	// 프레임 버퍼를 해제하는 함수
	void ReleaseFrameBuffer();
	// 래스터라이저 상태를 생성하는 함수
	void CreateRasterizerState();
	void ReleaseRasterizerState();
	// 렌더러에 사용된 모든 리소스를 해제하는 함수
	void Release();

	// 스왑 체인의 백 버퍼와 프론트 버퍼를 교체하여 화면에 출력
	void SwapBuffer();
	void CreateShader();
	void ReleaseShader();
	void Prepare();
	void PrepareShader();
	void RenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices);

	ID3D11Buffer* CreateVertexBuffer(FVertexSimple* vertices, UINT byteWidth);
	static void ReleaseVertexBuffer(ID3D11Buffer* vertexBuffer);
	void CreateConstantBuffer();
	void ReleaseConstantBuffer();
	void UpdateConstant(FVector Offset, float radius, float rotationAngle);

};