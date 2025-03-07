#include "Define.h"
#include "System.h"

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
	float X, Y, z;    // Position
	float r, g, b, a; // Color
};

#include "Sphere.h"
const FVector gravity(0.f, 0.000005f, 0.f);
/*
class URenderer
{
	struct FConstants
	{
		FVector Offset;
		float radius; 
		float rotationAngle;
	};
public:
	ID3D11VertexShader* SimpleVertexShader;
	ID3D11PixelShader* SimplePixelShader;
	ID3D11InputLayout* SimpleInputLayout;
	unsigned int Stride;
public:
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

	//void Prepare()
	//{
	//	DeviceContext->ClearRenderTargetView(FrameBufferRTV, ClearColor); // ���� Ÿ�� �信 ����� ���� ������ �����͸� ����

	//	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // ���� ���� ��� ����


	//	DeviceContext->OMSetRenderTargets(1, &FrameBufferRTV, nullptr); // ���� Ÿ�� ����(����۸� ����Ŵ)
	//	DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff); // ���� ���� ����, �⺻���� ������
	//}

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
	void UpdateConstant(FVector Offset, float radius, float rotationAngle)
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
*/
const float leftBorder = -1.0f;
const float rightBorder = 1.0f;
const float topBorder = -1.0f;
const float bottomBorder = 1.0f;
const float sphereRadius = 1.0f;
const float ballSpeed = 0.000005f;

float scaleMod = 0.1f;
bool bBlackHole = false;
FVector locBlackHole(0.0f, 0.0f, 0.0f);
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
	FVector Location;
	FVector Velocity;
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
		FVector newLocation;
		bool locationValid = false;

		// �ִ� �õ� Ƚ�� (�ʹ� ���� �õ��� ���� �ʵ��� ����)
		int maxAttempts = 10;
		int attempts = 0;
		
		//���� ���� ���� Ȯ��
		while (!locationValid && attempts < maxAttempts)
		{
			newLocation = FVector(((rand() % 2000) / 1000.0f) - 1.0f, ((rand() % 2000) / 1000.0f) - 1.0f, 0.f);


			UBall* pIter = NextBall;
			locationValid = true;
			while (pIter)
			{

				float distance = (newLocation - pIter->Location).Length();
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
			PossibleBall->Velocity = FVector(((float)(rand() % 100 - 50)) * ballSpeed, ((float)(rand() % 100 - 50)) * ballSpeed, 0.0f);
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
    private:
    void ResolveOverlap(FVector& pos1, FVector& pos2, float penetrationDepth) // ������ �� ����߸���
    {
        FVector normal = (pos1 - pos2).Normalize();

        float move1 = penetrationDepth * 0.55f; // 0.5������ �������� �浹 ó���� ���� ���� �ָ� ����߸�
        float move2 = penetrationDepth * 0.55f;

        pos1 = pos1 + normal * move1;
        pos2 = pos2 - normal * move2;
    }
	void BoundaryHandle() // ȭ�� ��� Ȯ�� �Լ�
	{
		if (Location.X < leftBorder + Radius)
		{
			Velocity.X *= -1.0f;
		}
		if (Location.X > rightBorder - Radius)
		{
			Velocity.X *= -1.0f;
		}
		if (Location.Y < topBorder + Radius)
		{
			Velocity.Y *= -1.0f;
		}
		if (Location.Y > bottomBorder - Radius)
		{
			Velocity.Y *= -1.0f;
		}
		if (Location.X < leftBorder + Radius)
		{
			Location.X = leftBorder + Radius;
		}
		if (Location.X > rightBorder - Radius)
		{
			Location.X = rightBorder - Radius;
		}
		if (Location.Y < topBorder + Radius)
		{
			Location.Y = topBorder + Radius;
		}
		if (Location.Y > bottomBorder - Radius)
		{
			Location.Y = bottomBorder - Radius;
		}
	}
	void CollisionHandle() // �浹 ó�� �Լ�
	{
		UBall* pIter = NextBall;
		while (pIter)
		{
			//���� ���� �浹
			float distance = (Location - pIter->Location).Length();
			float radiusSum = Radius + pIter->Radius;
			float penetrationDepth = radiusSum - distance;
			if (distance <= radiusSum)
			{
				FVector normal = (Location - pIter->Location).Normalize();
				ResolveOverlap(Location, pIter->Location, penetrationDepth);

				FVector relativeVelocity = Velocity - pIter->Velocity;

				float velocityAlongNormal = relativeVelocity.Dot(normal);

				if (velocityAlongNormal > 0) continue;

				float impulse = -2.f * velocityAlongNormal / (Mass + pIter->Mass);
				Velocity = Velocity + normal * (impulse * pIter->Mass);
				pIter->Velocity = pIter->Velocity - normal * (impulse * Mass);

				// ȸ�� �߰� 
				FVector tangent = FVector(-normal.Y, normal.X, 0); // ���� ����
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	bool bResult;

	USystem* System = new USystem;
	if (!System)
	{
		return 0;
	}

	// system ��ü�� �ʱ�ȭ�ϰ� run�� ȣ���Ѵ�.
	bResult = System->Initialize();
	if (bResult)
	{
		System->Run();
	}

	// system��ü�� �����ϰ� �޸𸮸� ��ȯ�Ѵ�.
	System->Release();
	delete System;
	System = 0;

	return 0;
}
/*

	///////////////////////////////
	///////////////////////////////
	///////////////////////////////
	///////////////////////////////

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
					locBlackHole.X -= 0.05f;
				}
				if (msg.wParam == VK_RIGHT)
				{
					locBlackHole.X += 0.05f;
				}
				if (msg.wParam == VK_UP)
				{
					locBlackHole.Y += 0.05f;
				}
				if (msg.wParam == VK_DOWN)
				{
					locBlackHole.Y -= 0.05f;
				}
				if (locBlackHole.X < leftBorder)
				{
					locBlackHole.X = leftBorder;
				}
				if (locBlackHole.X > rightBorder)
				{
					locBlackHole.X = rightBorder;
				}
				if (locBlackHole.Y < topBorder)
				{
					locBlackHole.Y = topBorder;
				}
				if (locBlackHole.Y > bottomBorder)
				{
					locBlackHole.Y = bottomBorder;
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
					float distance = (BlackHole->Location - ptmp->Location).Length();
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
*/