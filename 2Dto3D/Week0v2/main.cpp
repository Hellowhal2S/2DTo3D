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

#include "Vertex.h"
#include "VectorUtils.h"
#include "Renderer.h"
#include "ImGuiManager.h"

#define PI 3.14

#include "Sphere.h"

#pragma region �������� ���� �ڵ� �ּ� ó��
//const FVector gravity(0.f, 0.000005f, 0.f);
//
//const float leftBorder = -1.0f;
//const float rightBorder = 1.0f;
//const float topBorder = -1.0f;
//const float bottomBorder = 1.0f;
//const float sphereRadius = 1.0f;
//const float ballSpeed = 0.000005f;
//
//float scaleMod = 0.1f;
//bool bBlackHole = false;
//FVector locBlackHole(0.0f, 0.0f, 0.0f);
//float blackHolePower = 1.0f;
//bool bGravity = false;
//bool bRotate = false;
//bool bEating = false;
//bool bAutoSpawn = false;
//class UBall
//{
//public:
//	UBall() {}
//	~UBall() {}
//public:
//	// Ŭ���� �̸���, �Ʒ� �ΰ��� ���� �̸��� �������� �ʽ��ϴ�.
//	FVector Location;
//	FVector Velocity;
//	float Radius;
//	float Mass;
//	float AngularVelocity = 0.01f;
//	float RotationAngle = 0.1f;
//
//	UBall* NextBall;
//	UBall* PrevBall;
//	static INT32 ballCount;
//
//	float Index = 0;
//
//	bool CreateBall() // ���ο� �� ����
//	{
//		ballCount++;
//		UBall* PossibleBall  = new UBall;
//		// ������ ��ġ �ʱ�ȭ
//		FVector newLocation;
//		bool locationValid = false;
//
//		// �ִ� �õ� Ƚ�� (�ʹ� ���� �õ��� ���� �ʵ��� ����)
//		int maxAttempts = 10;
//		int attempts = 0;
//		
//		//���� ���� ���� Ȯ��
//		while (!locationValid && attempts < maxAttempts)
//		{
//			newLocation = FVector(((rand() % 2000) / 1000.0f) - 1.0f, ((rand() % 2000) / 1000.0f) - 1.0f, 0.f);
//
//
//			UBall* pIter = NextBall;
//			locationValid = true;
//			while (pIter)
//			{
//
//				float distance = (newLocation - pIter->Location).Magnitude();
//				float radiusSum = NextBall->Radius + pIter->Radius;
//				if (distance < radiusSum)  // ��ħ �߻�
//				{
//					locationValid = false; // ��ħ�� ������ ��ȿ���� ����
//					break;
//				}
//				pIter = pIter->NextBall;
//			}
//
//			attempts++;
//		}
//		// ���� ���� ��ġ�� ã�Ҵٸ� ����
//		if (locationValid)
//		{
//			PossibleBall->Location = newLocation;
//			PossibleBall->Velocity = FVector(((float)(rand() % 100 - 50)) * ballSpeed, ((float)(rand() % 100 - 50)) * ballSpeed, 0.0f);
//			PossibleBall->Radius = (sphereRadius * scaleMod) * (1.f - ((rand() % 1001) / 1000.0) * 0.9);
//			PossibleBall->Mass = PossibleBall->Radius * 100.0f;
//			PossibleBall->NextBall = NextBall;
//			PossibleBall->PrevBall = this;
//			if(NextBall)
//				NextBall->PrevBall = PossibleBall;
//			NextBall = PossibleBall;
//			return true;
//		}
//		else
//		{
//			delete PossibleBall;
//			ballCount--;
//			return false;
//		}
//	}
//	void DeleteRandomBall() // ���� ��ġ�� �� �����
//	{
//		int deleteIdx = (rand() % UBall::ballCount) + 1;
//		UBall* pIter = this;
//		int count = 1;
//		while (pIter)
//		{
//			UBall* pNext = pIter->NextBall;
//			if (pNext == nullptr)
//				break;
//			if (count == deleteIdx)
//			{
//				pIter->NextBall = pNext->NextBall;
//				delete pNext;
//				break;
//			}
//			pIter = pIter->NextBall;
//			count++;
//		}
//		ballCount--;
//	}
//	void DeleteBall() // ���� �� �����
//	{
//		if (PrevBall)
//		{
//			PrevBall->NextBall = NextBall;
//		}
//		if (NextBall)
//		{
//			NextBall->PrevBall = PrevBall;
//		}
//		ballCount--;
//		delete this;
//	}
//
//	void Update(float deltaTime) // ������Ʈ �Լ�
//	{
//		Move(deltaTime);
//		if(bRotate)
//			Rotate(deltaTime);
//	}
//private:
//	void ResolveOverlap(FVector& pos1, FVector& pos2, float penetrationDepth) // ������ �� ����߸���
//	{
//		FVector normal = (pos1 - pos2).Normalize();
//
//		float move1 = penetrationDepth * 0.55f; // 0.5������ �������� �浹 ó���� ���� ���� �ָ� ����߸�
//		float move2 = penetrationDepth * 0.55f;
//
//		pos1 = pos1 + normal * move1;
//		pos2 = pos2 - normal * move2;
//	}
//	void BoundaryHandle() // ȭ�� ��� Ȯ�� �Լ�
//	{
//		if (Location.x < leftBorder + Radius)
//		{
//			Velocity.x *= -1.0f;
//		}
//		if (Location.x > rightBorder - Radius)
//		{
//			Velocity.x *= -1.0f;
//		}
//		if (Location.y < topBorder + Radius)
//		{
//			Velocity.y *= -1.0f;
//		}
//		if (Location.y > bottomBorder - Radius)
//		{
//			Velocity.y *= -1.0f;
//		}
//		if (Location.x < leftBorder + Radius)
//		{
//			Location.x = leftBorder + Radius;
//		}
//		if (Location.x > rightBorder - Radius)
//		{
//			Location.x = rightBorder - Radius;
//		}
//		if (Location.y < topBorder + Radius)
//		{
//			Location.y = topBorder + Radius;
//		}
//		if (Location.y > bottomBorder - Radius)
//		{
//			Location.y = bottomBorder - Radius;
//		}
//	}
//	void CollisionHandle() // �浹 ó�� �Լ�
//	{
//		UBall* pIter = NextBall;
//		while (pIter)
//		{
//			//���� ���� �浹
//			float distance = (Location - pIter->Location).Magnitude();
//			float radiusSum = Radius + pIter->Radius;
//			float penetrationDepth = radiusSum - distance;
//			if (distance <= radiusSum)
//			{
//				FVector normal = (Location - pIter->Location).Normalize();
//				ResolveOverlap(Location, pIter->Location, penetrationDepth);
//
//				FVector relativeVelocity = Velocity - pIter->Velocity;
//
//				float velocityAlongNormal = relativeVelocity.Dot(normal);
//
//				if (velocityAlongNormal > 0) continue;
//
//				float impulse = -2.f * velocityAlongNormal / (Mass + pIter->Mass);
//				Velocity = Velocity + normal * (impulse * pIter->Mass);
//				pIter->Velocity = pIter->Velocity - normal * (impulse * Mass);
//
//				// ȸ�� �߰� 
//				FVector tangent = FVector(-normal.y, normal.x, 0); // ���� ����
//				float relativeTangentialVelocity = relativeVelocity.Dot(tangent);
//
//				float rotationalImpulse = relativeTangentialVelocity / (Radius + pIter->Radius);
//				AngularVelocity += rotationalImpulse / Radius;
//				pIter->AngularVelocity -= rotationalImpulse / pIter->Radius;
//			}
//			////////////////////////////////////////////////////////////////////////////
//
//			if (pIter->NextBall == nullptr)
//				break;
//			pIter = pIter->NextBall;
//		}
//	}
//	void Move(float deltaTime) // �̵� �Լ�
//	{
//		if (bGravity)
//			Velocity = Velocity - gravity*deltaTime;
//		if (bBlackHole)
//		{
//			Velocity = Velocity + (locBlackHole - Location).Normalize() * blackHolePower * 0.00001;
//		}
//		Location = Location + Velocity * deltaTime;
//		BoundaryHandle();
//		CollisionHandle();
//	}
//	void Rotate(float deltaTime) // ȸ�� �Լ�
//	{
//		RotationAngle += AngularVelocity * deltaTime;
//
//		// 2�� �Ѿ�� �ʵ��� ���� (���ʿ��� ū �� ����)
//		if (RotationAngle > 2 * PI)
//			RotationAngle -= 2 * PI;
//		else if (RotationAngle < -2 * PI)
//			RotationAngle += 2 * PI;
//	}
//
//};
//
//INT32 UBall::ballCount = 0;
#pragma endregion

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

	// InitImGUI();
	ImGuiManager::InitImGui(renderer, hWnd);

	UINT numVerticesSphere = sizeof(sphere_vertices) / sizeof(FVertexSimple);
	ID3D11Buffer* vertexBufferSphere = renderer.CreateVertexBuffer(sphere_vertices, sizeof(sphere_vertices));


	bool bIsExit = false;

#pragma region �������� ���� �ڵ� �ּ� ó��
	//���� ���� �ڵ�
	//UBall* HeadBall = new UBall; 
	//HeadBall->CreateBall();
	//UBall* BlackHole = new UBall;
	//BlackHole->Radius = 0.02f;
	//BlackHole->RotationAngle = 0;
	//static int numBalls = 1;  // ���� ���� �ʱⰪ
#pragma endregion
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
#pragma region �������� ���� �ڵ� �ּ� ó��
		//	if (bBlackHole) {
		//		if (msg.wParam == VK_LEFT)
		//		{
		//			locBlackHole.x -= 0.05f;
		//		}
		//		if (msg.wParam == VK_RIGHT)
		//		{
		//			locBlackHole.x += 0.05f;
		//		}
		//		if (msg.wParam == VK_UP)
		//		{
		//			locBlackHole.y += 0.05f;
		//		}
		//		if (msg.wParam == VK_DOWN)
		//		{
		//			locBlackHole.y -= 0.05f;
		//		}
		//		if (locBlackHole.x < leftBorder)
		//		{
		//			locBlackHole.x = leftBorder;
		//		}
		//		if (locBlackHole.x > rightBorder)
		//		{
		//			locBlackHole.x = rightBorder;
		//		}
		//		if (locBlackHole.y < topBorder)
		//		{
		//			locBlackHole.y = topBorder;
		//		}
		//		if (locBlackHole.y > bottomBorder)
		//		{
		//			locBlackHole.y = bottomBorder;
		//		}
		//	}
		//}
		//while (numBalls > UBall::ballCount)
		//{
		//	if (!HeadBall->CreateBall())
		//	{
		//		numBalls--;
		//	}
		//}
		//while (numBalls < UBall::ballCount)
		//{
		//	if (numBalls <= 0) {
		//		numBalls = 1;
		//		break;
		//	}
		//	HeadBall->DeleteRandomBall();
		//}
		//UBall* Iter = HeadBall->NextBall;
		//while (Iter)
		//{
		//	Iter->Update(elapsedTime);
		//	Iter = Iter->NextBall;
		//}

		////��Ȧ ����� ��ġ ���� 
		//if (bBlackHole) {
		//	BlackHole->Location = locBlackHole;
		//	if (bEating && UBall::ballCount > 1)
		//	{
		//		UBall* ptmp = HeadBall->NextBall;
		//		while (ptmp)
		//		{
		//			float distance = (BlackHole->Location - ptmp->Location).Magnitude();
		//			float radiusSum = BlackHole->Radius + ptmp->Radius;
		//			if (distance <= radiusSum)
		//			{
		//				if(BlackHole->Radius< 0.9f)
		//				BlackHole->Radius += ptmp->Mass * 0.0005;
		//				ptmp->DeleteBall();
		//				ptmp = HeadBall->NextBall;
		//				if(!bAutoSpawn)
		//					numBalls--;
		//			}
		//			ptmp = ptmp->NextBall;
		//		}
		//	}
#pragma endregion
		}
		// �غ� �۾�
		renderer.Prepare();
		renderer.PrepareShader();
#pragma region �������� ���� �ڵ� �ּ� ó��
		//Iter = HeadBall->NextBall;
		//while (Iter)
		//{
		//	renderer.UpdateConstant(Iter->Location , Iter->Radius, Iter->RotationAngle);
		//	// ������ ���ؽ� ���۸� �Ѱ� �������� ������ ��û
		//	renderer.RenderPrimitive(vertexBufferSphere, numVerticesSphere);
		//	Iter = Iter->NextBall;
		//}
		//if (bBlackHole) 
		//{
		//	renderer.UpdateConstant(BlackHole->Location, BlackHole->Radius, BlackHole->RotationAngle);
		//	renderer.RenderPrimitive(vertexBufferSphere, numVerticesSphere);
		//}
#pragma endregion
		// NewImGUIFrame()
		ImGuiManager::NewImGuiFrame();

		// ���� ImGui UI ��Ʈ�� �߰��� ImGui::NewFrame()�� ImGui::Render() ������ ���⿡ ��ġ�մϴ�.
		ImGui::Begin("Jungle Property Window");

		ImGui::Text("Hello Jungle World!");

#pragma region �������� ���� �ڵ� �ּ� ó��
		////ImGui::Text("%f", HeadBall->NextBall->Mass);
		//ImGui::Checkbox("Gravity", &bGravity);
		//ImGui::Checkbox("Rotate", &bRotate);
		//ImGui::Checkbox("BlackHole", &bBlackHole);
		//if (bBlackHole)
		//{
		//	ImGui::Indent(20.0f);
		//	if (BlackHole->Radius >= 0.9f) {
		//		ImGui::Text("Clear!!!!!!");
		//	}
		//	else 
		//		ImGui::Text("Score : %f", (BlackHole->Radius-0.02f)*10000);
		//	ImGui::Checkbox("Eating", &bEating);
		//	if (bEating)
		//	{
		//		ImGui::Indent(20.0f);
		//		ImGui::Checkbox("AutoSpawn", &bAutoSpawn);
		//		ImGui::Unindent(20.0f);
		//	}
		//	ImGui::PushItemWidth(160);
		//	ImGui::SliderFloat("BlackHole Power", &blackHolePower, .0f, 10.0f);
		//	ImGui::PopItemWidth();
		//	if (ImGui::Button("Restart"))
		//	{
		//		BlackHole->Radius = 0.02f;
		//	}
		//	ImGui::Unindent(20.0f);  
		//}


		//ImGui::PushItemWidth(80);
		//ImGui::InputInt("##balls", &numBalls);
		//ImGui::PopItemWidth();

		//ImGui::SameLine();
		//ImGui::Text("Number of Balls");
#pragma endregion

		ImGui::End();
		/////////////////////////////////////////////////////////////////////////
		ImGuiManager::RenderImGui();

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

#pragma region �������� ���� �ڵ� �ּ� ó��
	/*while (HeadBall != nullptr)
	{
		UBall* tmp = HeadBall->NextBall;
		delete HeadBall;
		HeadBall = tmp;
	}
	delete BlackHole;*/
#pragma endregion
	URenderer::ReleaseVertexBuffer(vertexBufferSphere);
	
	
	renderer.ReleaseConstantBuffer();
	renderer.ReleaseShader();
	renderer.Release();

	// ���⿡�� ImGui �Ҹ�
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	return 0;
}
