#pragma once
#include <Windows.h>

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_internal.h"
#include "ImGUI/imgui_impl_dx11.h"
#include "ImGUI/imgui_impl_win32.h"
#include "Define.h"
#include "UWorld.h"
#include "Sphere.h"
#include "UGraphicDevice.h"
#include "URenderer.h"
const float sphereRadius = 1.0f;


//class UBall
//{
//public:
//	UBall() {}
//	~UBall() {}
//public:
//	// 클래스 이름과, 아래 두개의 변수 이름은 변경하지 않습니다.
//	FVector3 Location;
//	FVector3 Velocity;
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
//	bool CreateBall() // 새로운 공 생성
//	{
//		ballCount++;
//		UBall* PossibleBall  = new UBall;
//		// 생성할 위치 초기화
//		FVector3 newLocation;
//		bool locationValid = false;
//
//		// 최대 시도 횟수 (너무 많은 시도가 되지 않도록 제한)
//		int maxAttempts = 10;
//		int attempts = 0;
//		
//		//생성 가능 여부 확인
//		while (!locationValid && attempts < maxAttempts)
//		{
//			newLocation = FVector3(((rand() % 2000) / 1000.0f) - 1.0f, ((rand() % 2000) / 1000.0f) - 1.0f, 0.f);
//
//
//			UBall* pIter = NextBall;
//			locationValid = true;
//			while (pIter)
//			{
//
//				float distance = (newLocation - pIter->Location).Magnitude();
//				float radiusSum = NextBall->Radius + pIter->Radius;
//				if (distance < radiusSum)  // 겹침 발생
//				{
//					locationValid = false; // 겹침이 있으면 유효하지 않음
//					break;
//				}
//				pIter = pIter->NextBall;
//			}
//
//			attempts++;
//		}
//		// 생성 가능 위치를 찾았다면 생성
//		if (locationValid)
//		{
//			PossibleBall->Location = newLocation;
//			PossibleBall->Velocity = FVector3(((float)(rand() % 100 - 50)) * ballSpeed, ((float)(rand() % 100 - 50)) * ballSpeed, 0.0f);
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
//	void DeleteRandomBall() // 랜덤 위치의 공 지우기
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
//	void DeleteBall() // 지금 공 지우기
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
//	void Update(float deltaTime) // 업데이트 함수
//	{
//		Move(deltaTime);
//		if(bRotate)
//			Rotate(deltaTime);
//	}
//};

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

	UGraphicsDevice graphicDevice;
	graphicDevice.Initialize(hWnd);
	URenderer renderer;
	renderer.Initialize(&graphicDevice);


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init((void*)hWnd);
	ImGui_ImplDX11_Init(graphicDevice.Device, graphicDevice.DeviceContext);

	UINT numVerticesSphere = sizeof(sphere_vertices) / sizeof(FVertexSimple);
	ID3D11Buffer* vertexBufferSphere = renderer.CreateVertexBuffer(sphere_vertices, sizeof(sphere_vertices));

	UWorld* World = new UWorld;
	World->Initialize();

	bool bIsExit = false;

	const int targetFPS = 60;
	const double targetFrameTime = 1000.0 / targetFPS; // 한 프레임의 목표 시간 (밀리초 단위)
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
			TranslateMessage(&msg); // 키보드 입력 메시지를 문자메시지로 변경
			DispatchMessage(&msg); // 메시지를 WndProc에 전달

			if (msg.message == WM_QUIT)
			{
				bIsExit = true;
				break;
			}
		}

		World->Update();


		// 준비 작업
		renderer.Prepare();
		renderer.PrepareShader();

		renderer.UpdateConstant(FVector(), 1, 10);
		renderer.RenderPrimitive(vertexBufferSphere, numVerticesSphere);

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// 이후 ImGui UI 컨트롤 추가는 ImGui::NewFrame()과 ImGui::Render() 사이인 여기에 위치합니다.
		ImGui::Begin("Jungle Property Window");

		ImGui::Text("Hello Jungle World!");

		//ImGui::Text("%f", HeadBall->NextBall->Mass);


		ImGui::SameLine();
		ImGui::Text("Number of Balls");

		ImGui::End();
		/////////////////////////////////////////////////////////////////////////
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		//버퍼 교체
		graphicDevice.SwapBuffer();
		do
		{
			Sleep(0);

			// 루프 종료 시간 기록
			QueryPerformanceCounter(&endTime);

			// 한 프레임이 소요된 시간 계산 (밀리초 단위로 변환)
			elapsedTime = (endTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart;

		} while (elapsedTime < targetFrameTime);
	}


	delete World;
	

	renderer.ReleaseVertexBuffer(vertexBufferSphere);
	renderer.Release();

	graphicDevice.Release();
	// 여기에서 ImGui 소멸
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	return 0;
}
