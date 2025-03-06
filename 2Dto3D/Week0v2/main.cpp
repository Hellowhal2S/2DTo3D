#include <Windows.h>

#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

#include <d3d11.h>
#include <d3dcompiler.h>

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_internal.h"
#include "ImGUI/imgui_impl_dx11.h"
#include "ImGUI/imgui_impl_win32.h"
#define PI 3.14
struct FVertexSimple
{
	float x, y, z;    // Position
	float r, g, b, a; // Color
};
struct FVector3
{
	float x, y, z;
	FVector3(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}

	FVector3 operator-(const FVector3& other) const {
		return FVector3(x - other.x, y - other.y, z - other.z);
	}
	FVector3 operator+(const FVector3& other) const {
		return FVector3(x + other.x, y + other.y, z + other.z);
	}

	// ���� ����
	float Dot(const FVector3& other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	// ���� ũ��
	float Magnitude() const {
		return sqrt(x * x + y * y + z * z);
	}

	// ���� ����ȭ
	FVector3 Normalize() const {
		float mag = Magnitude();
		return (mag > 0) ? FVector3(x / mag, y / mag, z / mag) : FVector3(0, 0, 0);
	}

	// ��Į�� ����
	FVector3 operator*(float scalar) const {
		return FVector3(x * scalar, y * scalar, z * scalar);
	}
};

#include "Sphere.h"
const FVector3 gravity(0.f, 0.000005f, 0.f);

class URenderer
{
	struct FConstants
	{
		FVector3 Offset;
		float radius; 
		float rotationAngle;
	};
public:
	ID3D11Device* Device = nullptr;
	ID3D11DeviceContext* DeviceContext = nullptr;
	IDXGISwapChain* SwapChain = nullptr;
	// �������� �ʿ��� ���ҽ� �� ���¸� �����ϱ� ���� ������
	ID3D11Texture2D* FrameBuffer = nullptr; // ȭ�� ��¿� �ؽ�ó
	ID3D11RenderTargetView* FrameBufferRTV = nullptr; // �ؽ�ó�� ���� Ÿ������ ����ϴ� ��
	ID3D11RasterizerState* RasterizerState = nullptr; // �����Ͷ����� ����(�ø�, ä��� ��� �� ����)
	ID3D11Buffer* ConstantBuffer = nullptr; // ���̴��� �����͸� �����ϱ� ���� ��� ����

	FLOAT ClearColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f }; // ȭ���� �ʱ�ȭ(clear)�� �� ����� ���� (RGBA)
	D3D11_VIEWPORT ViewportInfo; // ������ ������ �����ϴ� ����Ʈ ����

	ID3D11VertexShader* SimpleVertexShader;
	ID3D11PixelShader* SimplePixelShader;
	ID3D11InputLayout* SimpleInputLayout;
	unsigned int Stride;
public:

	// ������ �ʱ�ȭ �Լ�
	void Create(HWND hWindow)
	{
		// Direct3D ��ġ �� ���� ü�� ����
		CreateDeviceAndSwapChain(hWindow);

		// ������ ���� ����
		CreateFrameBuffer();

		// �����Ͷ����� ���� ����
		CreateRasterizerState();

		// ���� ���ٽ� ���� �� ���� ���´� �� �ڵ忡���� �ٷ��� ����
	}
	// ��ġ �� ����ü�� ����
	void CreateDeviceAndSwapChain(HWND hWindow)
	{
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
	void ReleaseDeviceAndSwapChain()
	{
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
	// ������ ���۸� �����ϴ� �Լ�
	void CreateFrameBuffer()
	{
		// ���� ü�����κ��� �� ���� �ؽ�ó ��������
		SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&FrameBuffer);

		// ���� Ÿ�� �� ����
		D3D11_RENDER_TARGET_VIEW_DESC framebufferRTVdesc = {};
		framebufferRTVdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB; // ���� ����
		framebufferRTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; // 2D �ؽ�ó

		Device->CreateRenderTargetView(FrameBuffer, &framebufferRTVdesc, &FrameBufferRTV);
	}
	// ������ ���۸� �����ϴ� �Լ�
	void ReleaseFrameBuffer()
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

	// �����Ͷ����� ���¸� �����ϴ� �Լ�
	void CreateRasterizerState()
	{
		D3D11_RASTERIZER_DESC rasterizerdesc = {};
		rasterizerdesc.FillMode = D3D11_FILL_SOLID; // ä��� ���
		rasterizerdesc.CullMode = D3D11_CULL_BACK; // �� ���̽� �ø�

		Device->CreateRasterizerState(&rasterizerdesc, &RasterizerState);
	}
	void ReleaseRasterizerState()
	{
		if (RasterizerState)
		{
			RasterizerState->Release();
			RasterizerState = nullptr;
		}
	}

	// �������� ���� ��� ���ҽ��� �����ϴ� �Լ�
	void Release()
	{
		RasterizerState->Release();

		// ���� Ÿ���� �ʱ�ȭ
		DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

		ReleaseFrameBuffer();
		ReleaseDeviceAndSwapChain();
	}

	// ���� ü���� �� ���ۿ� ����Ʈ ���۸� ��ü�Ͽ� ȭ�鿡 ���
	void SwapBuffer()
	{
		SwapChain->Present(1, 0); // 1: VSync Ȱ��ȭ
	}

	void CreateShader()
	{
		ID3DBlob* vertexshaderCSO;
		ID3DBlob* pixelshaderCSO;

		D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainVS", "vs_5_0", 0, 0, &vertexshaderCSO, nullptr);

		Device->CreateVertexShader(vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), nullptr, &SimpleVertexShader);

		D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainPS", "ps_5_0", 0, 0, &pixelshaderCSO, nullptr);

		Device->CreatePixelShader(pixelshaderCSO->GetBufferPointer(), pixelshaderCSO->GetBufferSize(), nullptr, &SimplePixelShader);

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		Device->CreateInputLayout(layout, ARRAYSIZE(layout), vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), &SimpleInputLayout);

		Stride = sizeof(FVertexSimple);

		vertexshaderCSO->Release();
		pixelshaderCSO->Release();
	}

	void ReleaseShader()
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

	void Prepare()
	{
		DeviceContext->ClearRenderTargetView(FrameBufferRTV, ClearColor); // ���� Ÿ�� �信 ����� ���� ������ �����͸� ����

		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // ���� ���� ��� ����

		DeviceContext->RSSetViewports(1, &ViewportInfo); // GPU�� ȭ���� �������� ���� ����
		DeviceContext->RSSetState(RasterizerState); //������ ������ ���� ����

		DeviceContext->OMSetRenderTargets(1, &FrameBufferRTV, nullptr); // ���� Ÿ�� ����(����۸� ����Ŵ)
		DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff); // ���� ���� ����, �⺻���� ������
	}

	void PrepareShader()
	{
		DeviceContext->VSSetShader(SimpleVertexShader, nullptr, 0);
		DeviceContext->PSSetShader(SimplePixelShader, nullptr, 0);
		DeviceContext->IASetInputLayout(SimpleInputLayout);
		
		if (ConstantBuffer)
		{
			DeviceContext->VSSetConstantBuffers(0, 1, &ConstantBuffer);
		}
	}

	void RenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices) 
	{
		UINT offset = 0;
		DeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &Stride, &offset);

		DeviceContext->Draw(numVertices, 0);
	}
	
	ID3D11Buffer* CreateVertexBuffer(FVertexSimple* vertices, UINT byteWidth)
	{
		// 2. Create a vertex buffer
		D3D11_BUFFER_DESC vertexbufferdesc = {};
		vertexbufferdesc.ByteWidth = byteWidth;
		vertexbufferdesc.Usage = D3D11_USAGE_IMMUTABLE; // will never be updated 
		vertexbufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexbufferSRD = { vertices };

		ID3D11Buffer* vertexBuffer;

		Device->CreateBuffer(&vertexbufferdesc, &vertexbufferSRD, &vertexBuffer);

		return vertexBuffer;
	}

	void ReleaseVertexBuffer(ID3D11Buffer* vertexBuffer)
	{
		vertexBuffer->Release();
	}

	void CreateConstantBuffer()
	{
		D3D11_BUFFER_DESC constantbufferdesc = {};
		constantbufferdesc.ByteWidth = sizeof(FConstants) + 0xf & 0xfffffff0; // ensure constant buffer size is multiple of 16 bytes
		constantbufferdesc.Usage = D3D11_USAGE_DYNAMIC; // will be updated from CPU every frame
		constantbufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constantbufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		Device->CreateBuffer(&constantbufferdesc, nullptr, &ConstantBuffer);
	}
	void ReleaseConstantBuffer()
	{
		if (ConstantBuffer)
		{
			ConstantBuffer->Release();
			ConstantBuffer = nullptr;
		}
	}
	void UpdateConstant(FVector3 Offset, float radius, float rotationAngle)
	{
		if (ConstantBuffer)
		{
			D3D11_MAPPED_SUBRESOURCE constantbufferMSR;// GPU�� �޸� �ּ� ����

			DeviceContext->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantbufferMSR); // update constant buffer every frame
			FConstants* constants = (FConstants*)constantbufferMSR.pData; //GPU �޸� ���� ����
			{
				constants->Offset = Offset;
				constants->radius = radius;
				constants->rotationAngle = rotationAngle;
			}
			DeviceContext->Unmap(ConstantBuffer, 0); // GPU�� �ٽ� ��밡���ϰ� �����
		}
	}

};
const float leftBorder = -1.0f;
const float rightBorder = 1.0f;
const float topBorder = -1.0f;
const float bottomBorder = 1.0f;
const float sphereRadius = 1.0f;
const float ballSpeed = 0.000005f;

float scaleMod = 0.1f;
bool bBlackHole = false;
FVector3 locBlackHole(0.0f, 0.0f, 0.0f);
float blackHolePower = 1.0f;
bool bGravity = false;
bool bRotate = false;
bool bEating = false;
bool bAutoSpawn = false;
class UBall
{
public:
	UBall() {}
	~UBall() {}
public:
	// Ŭ���� �̸���, �Ʒ� �ΰ��� ���� �̸��� �������� �ʽ��ϴ�.
	FVector3 Location;
	FVector3 Velocity;
	float Radius;
	float Mass;
	float AngularVelocity = 0.01f;
	float RotationAngle = 0.1f;

	UBall* NextBall;
	UBall* PrevBall;
	static INT32 ballCount;

	float Index = 0;

	bool CreateBall() // ���ο� �� ����
	{
		ballCount++;
		UBall* PossibleBall  = new UBall;
		// ������ ��ġ �ʱ�ȭ
		FVector3 newLocation;
		bool locationValid = false;

		// �ִ� �õ� Ƚ�� (�ʹ� ���� �õ��� ���� �ʵ��� ����)
		int maxAttempts = 10;
		int attempts = 0;
		
		//���� ���� ���� Ȯ��
		while (!locationValid && attempts < maxAttempts)
		{
			newLocation = FVector3(((rand() % 2000) / 1000.0f) - 1.0f, ((rand() % 2000) / 1000.0f) - 1.0f, 0.f);


			UBall* pIter = NextBall;
			locationValid = true;
			while (pIter)
			{

				float distance = (newLocation - pIter->Location).Magnitude();
				float radiusSum = NextBall->Radius + pIter->Radius;
				if (distance < radiusSum)  // ��ħ �߻�
				{
					locationValid = false; // ��ħ�� ������ ��ȿ���� ����
					break;
				}
				pIter = pIter->NextBall;
			}

			attempts++;
		}
		// ���� ���� ��ġ�� ã�Ҵٸ� ����
		if (locationValid)
		{
			PossibleBall->Location = newLocation;
			PossibleBall->Velocity = FVector3(((float)(rand() % 100 - 50)) * ballSpeed, ((float)(rand() % 100 - 50)) * ballSpeed, 0.0f);
			PossibleBall->Radius = (sphereRadius * scaleMod) * (1.f - ((rand() % 1001) / 1000.0) * 0.9);
			PossibleBall->Mass = PossibleBall->Radius * 100.0f;
			PossibleBall->NextBall = NextBall;
			PossibleBall->PrevBall = this;
			if(NextBall)
				NextBall->PrevBall = PossibleBall;
			NextBall = PossibleBall;
			return true;
		}
		else
		{
			delete PossibleBall;
			ballCount--;
			return false;
		}
	}
	void DeleteRandomBall() // ���� ��ġ�� �� �����
	{
		int deleteIdx = (rand() % UBall::ballCount) + 1;
		UBall* pIter = this;
		int count = 1;
		while (pIter)
		{
			UBall* pNext = pIter->NextBall;
			if (pNext == nullptr)
				break;
			if (count == deleteIdx)
			{
				pIter->NextBall = pNext->NextBall;
				delete pNext;
				break;
			}
			pIter = pIter->NextBall;
			count++;
		}
		ballCount--;
	}
	void DeleteBall() // ���� �� �����
	{
		if (PrevBall)
		{
			PrevBall->NextBall = NextBall;
		}
		if (NextBall)
		{
			NextBall->PrevBall = PrevBall;
		}
		ballCount--;
		delete this;
	}

	void Update(float deltaTime) // ������Ʈ �Լ�
	{
		Move(deltaTime);
		if(bRotate)
			Rotate(deltaTime);
	}
private:
	void ResolveOverlap(FVector3& pos1, FVector3& pos2, float penetrationDepth) // ������ �� ����߸���
	{
		FVector3 normal = (pos1 - pos2).Normalize();

		float move1 = penetrationDepth * 0.55f; // 0.5������ �������� �浹 ó���� ���� ���� �ָ� ����߸�
		float move2 = penetrationDepth * 0.55f;

		pos1 = pos1 + normal * move1;
		pos2 = pos2 - normal * move2;
	}
	void BoundaryHandle() // ȭ�� ��� Ȯ�� �Լ�
	{
		if (Location.x < leftBorder + Radius)
		{
			Velocity.x *= -1.0f;
		}
		if (Location.x > rightBorder - Radius)
		{
			Velocity.x *= -1.0f;
		}
		if (Location.y < topBorder + Radius)
		{
			Velocity.y *= -1.0f;
		}
		if (Location.y > bottomBorder - Radius)
		{
			Velocity.y *= -1.0f;
		}
		if (Location.x < leftBorder + Radius)
		{
			Location.x = leftBorder + Radius;
		}
		if (Location.x > rightBorder - Radius)
		{
			Location.x = rightBorder - Radius;
		}
		if (Location.y < topBorder + Radius)
		{
			Location.y = topBorder + Radius;
		}
		if (Location.y > bottomBorder - Radius)
		{
			Location.y = bottomBorder - Radius;
		}
	}
	void CollisionHandle() // �浹 ó�� �Լ�
	{
		UBall* pIter = NextBall;
		while (pIter)
		{
			//���� ���� �浹
			float distance = (Location - pIter->Location).Magnitude();
			float radiusSum = Radius + pIter->Radius;
			float penetrationDepth = radiusSum - distance;
			if (distance <= radiusSum)
			{
				FVector3 normal = (Location - pIter->Location).Normalize();
				ResolveOverlap(Location, pIter->Location, penetrationDepth);

				FVector3 relativeVelocity = Velocity - pIter->Velocity;

				float velocityAlongNormal = relativeVelocity.Dot(normal);

				if (velocityAlongNormal > 0) continue;

				float impulse = -2.f * velocityAlongNormal / (Mass + pIter->Mass);
				Velocity = Velocity + normal * (impulse * pIter->Mass);
				pIter->Velocity = pIter->Velocity - normal * (impulse * Mass);

				// ȸ�� �߰� 
				FVector3 tangent = FVector3(-normal.y, normal.x, 0); // ���� ����
				float relativeTangentialVelocity = relativeVelocity.Dot(tangent);

				float rotationalImpulse = relativeTangentialVelocity / (Radius + pIter->Radius);
				AngularVelocity += rotationalImpulse / Radius;
				pIter->AngularVelocity -= rotationalImpulse / pIter->Radius;
			}
			////////////////////////////////////////////////////////////////////////////

			if (pIter->NextBall == nullptr)
				break;
			pIter = pIter->NextBall;
		}
	}
	void Move(float deltaTime) // �̵� �Լ�
	{
		if (bGravity)
			Velocity = Velocity - gravity*deltaTime;
		if (bBlackHole)
		{
			Velocity = Velocity + (locBlackHole - Location).Normalize() * blackHolePower * 0.00001;
		}
		Location = Location + Velocity * deltaTime;
		BoundaryHandle();
		CollisionHandle();
	}
	void Rotate(float deltaTime) // ȸ�� �Լ�
	{
		RotationAngle += AngularVelocity * deltaTime;

		// 2�� �Ѿ�� �ʵ��� ���� (���ʿ��� ū �� ����)
		if (RotationAngle > 2 * PI)
			RotationAngle -= 2 * PI;
		else if (RotationAngle < -2 * PI)
			RotationAngle += 2 * PI;
	}

};

INT32 UBall::ballCount = 0;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
	{
		return true;
	}
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WCHAR WindowClass[] = L"JungleWindowClass";

	WCHAR Title[] = L"Game Tech Lab";

	WNDCLASSW wndclass = { 0, WndProc, 0,0,0,0,0,0,0, WindowClass };

	RegisterClassW(&wndclass);

	HWND hWnd = CreateWindowExW(0, WindowClass, Title, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1024, 1024,
		nullptr, nullptr, hInstance, nullptr);

	URenderer renderer;

	// ��ġ �� �⺻ �ٷ� �������� ����
	renderer.Create(hWnd);
	renderer.CreateShader();
	renderer.CreateConstantBuffer();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init((void*)hWnd);
	ImGui_ImplDX11_Init(renderer.Device, renderer.DeviceContext);

	UINT numVerticesSphere = sizeof(sphere_vertices) / sizeof(FVertexSimple);
	ID3D11Buffer* vertexBufferSphere = renderer.CreateVertexBuffer(sphere_vertices, sizeof(sphere_vertices));


	bool bIsExit = false;

	//���� ���� �ڵ�
	UBall* HeadBall = new UBall; 
	HeadBall->CreateBall();
	UBall* BlackHole = new UBall;
	BlackHole->Radius = 0.02f;
	BlackHole->RotationAngle = 0;
	static int numBalls = 1;  // ���� ���� �ʱⰪ

	const int targetFPS = 60;
	const double targetFrameTime = 1000.0 / targetFPS; // �� �������� ��ǥ �ð� (�и��� ����)
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	LARGE_INTEGER startTime, endTime;
	double elapsedTime = 1.0;
	while (bIsExit == false)
	{
		QueryPerformanceCounter(&startTime);

		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg); // Ű���� �Է� �޽����� ���ڸ޽����� ����
			DispatchMessage(&msg); // �޽����� WndProc�� ����

			if (msg.message == WM_QUIT)
			{
				bIsExit = true;
				break;
			}
			if (bBlackHole) {
				if (msg.wParam == VK_LEFT)
				{
					locBlackHole.x -= 0.05f;
				}
				if (msg.wParam == VK_RIGHT)
				{
					locBlackHole.x += 0.05f;
				}
				if (msg.wParam == VK_UP)
				{
					locBlackHole.y += 0.05f;
				}
				if (msg.wParam == VK_DOWN)
				{
					locBlackHole.y -= 0.05f;
				}
				if (locBlackHole.x < leftBorder)
				{
					locBlackHole.x = leftBorder;
				}
				if (locBlackHole.x > rightBorder)
				{
					locBlackHole.x = rightBorder;
				}
				if (locBlackHole.y < topBorder)
				{
					locBlackHole.y = topBorder;
				}
				if (locBlackHole.y > bottomBorder)
				{
					locBlackHole.y = bottomBorder;
				}
			}
		}
		while (numBalls > UBall::ballCount)
		{
			if (!HeadBall->CreateBall())
			{
				numBalls--;
			}
		}
		while (numBalls < UBall::ballCount)
		{
			if (numBalls <= 0) {
				numBalls = 1;
				break;
			}
			HeadBall->DeleteRandomBall();
		}
		UBall* Iter = HeadBall->NextBall;
		while (Iter)
		{
			Iter->Update(elapsedTime);
			Iter = Iter->NextBall;
		}

		//��Ȧ ����� ��ġ ���� 
		if (bBlackHole) {
			BlackHole->Location = locBlackHole;
			if (bEating && UBall::ballCount > 1)
			{
				UBall* ptmp = HeadBall->NextBall;
				while (ptmp)
				{
					float distance = (BlackHole->Location - ptmp->Location).Magnitude();
					float radiusSum = BlackHole->Radius + ptmp->Radius;
					if (distance <= radiusSum)
					{
						if(BlackHole->Radius< 0.9f)
						BlackHole->Radius += ptmp->Mass * 0.0005;
						ptmp->DeleteBall();
						ptmp = HeadBall->NextBall;
						if(!bAutoSpawn)
							numBalls--;
					}
					ptmp = ptmp->NextBall;
				}
			}
		}
		// �غ� �۾�
		renderer.Prepare();
		renderer.PrepareShader();

		Iter = HeadBall->NextBall;
		while (Iter)
		{
			renderer.UpdateConstant(Iter->Location , Iter->Radius, Iter->RotationAngle);
			// ������ ���ؽ� ���۸� �Ѱ� �������� ������ ��û
			renderer.RenderPrimitive(vertexBufferSphere, numVerticesSphere);
			Iter = Iter->NextBall;
		}
		if (bBlackHole) 
		{
			renderer.UpdateConstant(BlackHole->Location, BlackHole->Radius, BlackHole->RotationAngle);
			renderer.RenderPrimitive(vertexBufferSphere, numVerticesSphere);
		}
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// ���� ImGui UI ��Ʈ�� �߰��� ImGui::NewFrame()�� ImGui::Render() ������ ���⿡ ��ġ�մϴ�.
		ImGui::Begin("Jungle Property Window");

		ImGui::Text("Hello Jungle World!");

		//ImGui::Text("%f", HeadBall->NextBall->Mass);
		ImGui::Checkbox("Gravity", &bGravity);
		ImGui::Checkbox("Rotate", &bRotate);
		ImGui::Checkbox("BlackHole", &bBlackHole);
		if (bBlackHole)
		{
			ImGui::Indent(20.0f);
			if (BlackHole->Radius >= 0.9f) {
				ImGui::Text("Clear!!!!!!");
			}
			else 
				ImGui::Text("Score : %f", (BlackHole->Radius-0.02f)*10000);
			ImGui::Checkbox("Eating", &bEating);
			if (bEating)
			{
				ImGui::Indent(20.0f);
				ImGui::Checkbox("AutoSpawn", &bAutoSpawn);
				ImGui::Unindent(20.0f);
			}
			ImGui::PushItemWidth(160);
			ImGui::SliderFloat("BlackHole Power", &blackHolePower, .0f, 10.0f);
			ImGui::PopItemWidth();
			if (ImGui::Button("Restart"))
			{
				BlackHole->Radius = 0.02f;
			}
			ImGui::Unindent(20.0f);  
		}


		ImGui::PushItemWidth(80);
		ImGui::InputInt("##balls", &numBalls);
		ImGui::PopItemWidth();

		ImGui::SameLine();
		ImGui::Text("Number of Balls");

		ImGui::End();
		/////////////////////////////////////////////////////////////////////////
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		//���� ��ü
		renderer.SwapBuffer();
		do
		{
			Sleep(0);

			// ���� ���� �ð� ���
			QueryPerformanceCounter(&endTime);

			// �� �������� �ҿ�� �ð� ��� (�и��� ������ ��ȯ)
			elapsedTime = (endTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart;

		} while (elapsedTime < targetFrameTime);
	}

	while (HeadBall != nullptr)
	{
		UBall* tmp = HeadBall->NextBall;
		delete HeadBall;
		HeadBall = tmp;
	}
	delete BlackHole;
	renderer.ReleaseVertexBuffer(vertexBufferSphere);
	
	
	renderer.ReleaseConstantBuffer();
	renderer.ReleaseShader();
	renderer.Release();

	// ���⿡�� ImGui �Ҹ�
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	return 0;
}
