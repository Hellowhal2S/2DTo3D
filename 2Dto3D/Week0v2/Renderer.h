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
	// �������� �ʿ��� ���ҽ� �� ���¸� �����ϱ� ���� ������
	ID3D11Texture2D* FrameBuffer = nullptr; // ȭ�� ��¿� �ؽ�ó
	ID3D11RenderTargetView* FrameBufferRTV = nullptr; // �ؽ�ó�� ���� Ÿ������ ����ϴ� ��
	ID3D11RasterizerState* RasterizerState = nullptr; // �����Ͷ����� ����(�ø�, ä��� ��� �� ����)
	ID3D11Buffer* ConstantBuffer = nullptr; // ���̴��� �����͸� �����ϱ� ���� ��� ����

	FLOAT ClearColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f }; // ȭ���� �ʱ�ȭ(clear)�� �� ����� ���� (RGBA)

	ID3D11VertexShader* SimpleVertexShader;
	ID3D11PixelShader* SimplePixelShader;
	ID3D11InputLayout* SimpleInputLayout;
	unsigned int Stride;
public:

	// ������ �ʱ�ȭ �Լ�
	void Create(HWND hWindow);
	// ������ ���۸� �����ϴ� �Լ�
	void CreateFrameBuffer();
	// ������ ���۸� �����ϴ� �Լ�
	void ReleaseFrameBuffer();
	// �����Ͷ����� ���¸� �����ϴ� �Լ�
	void CreateRasterizerState();
	void ReleaseRasterizerState();
	// �������� ���� ��� ���ҽ��� �����ϴ� �Լ�
	void Release();

	// ���� ü���� �� ���ۿ� ����Ʈ ���۸� ��ü�Ͽ� ȭ�鿡 ���
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