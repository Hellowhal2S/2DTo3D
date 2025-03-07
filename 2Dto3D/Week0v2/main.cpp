#pragma once
#include <Windows.h>

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_internal.h"
#include "ImGUI/imgui_impl_dx11.h"
#include "ImGUI/imgui_impl_win32.h"
#include "Define.h"
#include "World.h"
#include "GraphicDevice.h"
#include "Renderer.h"
#include "JungleMath.h"
#include "CameraComponent.h"

#include "Sphere.h"
#include "Cube.h"
int screenWidth = 1024; // 예시 값
int screenHeight = 1024; // 예시 값

UGraphicsDevice graphicDevice;

void ScreenToRay(float screenX, float screenY, const FMatrix& viewMatrix, const FMatrix& projectionMatrix,
	FVector& rayOrigin, FVector& rayDir)
{
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	graphicDevice.DeviceContext->RSGetViewports(&numViewports, &viewport);
	float screenWidth = viewport.Width;
	float screenHeight = viewport.Height;
	float x = (2.0f * screenX) / screenWidth - 1.0f;
	float y = 1.0f - (2.0f * screenY) / screenHeight;

	// 프로젝션 역행렬 계산
	FMatrix inverseProj = FMatrix::Inverse(projectionMatrix);

	// NDC에서 뷰 공간으로 변환
	FVector4 nearPoint = inverseProj.TransformFVector4(FVector4(x, y, 0.0f, 1.0f));
	FVector4 farPoint = inverseProj.TransformFVector4(FVector4(x, y, 1.0f, 1.0f));

	// W를 1로 정규화
	nearPoint =  nearPoint / nearPoint.a;
	farPoint  =  farPoint / farPoint.a;

	// 뷰 행렬을 반영하여 월드 공간으로 변환
	FMatrix inverseView = FMatrix::Inverse(viewMatrix);
	FVector nearWorld = inverseView.TransformPosition(FVector(nearPoint.x, nearPoint.y, nearPoint.z));
	FVector farWorld = inverseView.TransformPosition(FVector(farPoint.x, farPoint.y, farPoint.z));

	// 레이의 시작점과 방향 계산
	rayOrigin = nearWorld;
	rayDir = farWorld - nearWorld;
	rayDir = rayDir.Normalize();

	char message[256];
	sprintf_s(message, "Ray Origin: (%.2f, %.2f, %.2f)\nRay Direction: (%.2f, %.2f, %.2f)",
		rayOrigin.x, rayOrigin.y, rayOrigin.z, rayDir.x, rayDir.y, rayDir.z);
	//MessageBoxA(nullptr, message, "ScreenToRay Output", MB_OK);
}

bool RayIntersectsSphere(const FVector& rayOrigin, const FVector& rayDir,
	const FVector& sphereCenter, float sphereRadius)
{
	// 레이와 구체의 교차 여부 계산
	FVector oc;
	oc.x = rayOrigin.x - sphereCenter.x;
	oc.y = rayOrigin.y - sphereCenter.y;
	oc.z = rayOrigin.z - sphereCenter.z;

	float b = 2.0f * (rayDir.x * oc.x + rayDir.y * oc.y + rayDir.z * oc.z);
	float c = (oc.x * oc.x + oc.y * oc.y + oc.z * oc.z) - sphereRadius * sphereRadius;

	float discriminant = b * b - 4.0f * c;
	return discriminant > 0;
}


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

	UINT numVerticesCube = sizeof(cube_vertices) / sizeof(FVertexSimple);
	ID3D11Buffer* vertexBufferCube = renderer.CreateVertexBuffer(cube_vertices, sizeof(cube_vertices));


	UWorld* World = new UWorld;
	World->Initialize();
	UCameraComponent* Camera = static_cast<UCameraComponent*>(World->GetCamera());

	bool bIsExit = false;
	static float fov = 60.0f;

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

		World->Update(elapsedTime);

		FMatrix View = JungleMath::CreateViewMatrix(Camera->GetLocation(), Camera->GetLocation() + Camera->GetForwardVector(), {0, 1, 0});
		FMatrix Projection = JungleMath::CreateProjectionMatrix(
			fov * (3.141592f / 180.0f),
			1.0f,  // 1:1 비율로 변경
			0.1f,
			1000.0f
		);
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			POINT mousePos;
			GetCursorPos(&mousePos);
			ScreenToClient(hWnd, &mousePos);

			FVector rayOrigin;
			FVector rayDir;
			ScreenToRay(mousePos.x, mousePos.y, View, Projection, rayOrigin, rayDir);
			UObject* Possible = nullptr;
			for (auto iter = World->GetSphreList().begin(); iter != World->GetSphreList().end();++iter)
			{
				if (RayIntersectsSphere(rayOrigin, rayDir, (*iter)->GetLocation(), 1.0f))
				{
					if(!Possible)
						Possible = (*iter);
					else if (Possible->GetLocation().Distance(rayOrigin) > ((*iter)->GetLocation().Distance(rayOrigin)))
						Possible = (*iter);
				}
			}
			if (Possible)
				World->SetPickingObj(Possible);
		}
		// 준비 작업a
		renderer.Prepare();
		renderer.PrepareShader();


		for (auto iter = World->GetSphreList().begin(); iter != World->GetSphreList().end();++iter)
		{
			FMatrix Model = JungleMath::CreateModelMatrix((*iter)->GetLocation(), (*iter)->GetRotation(), (*iter)->GetScale());

			// 최종 MVP 행렬
			FMatrix MVP = Model * View * Projection;

			renderer.UpdateConstant(MVP);
			renderer.RenderPrimitive(vertexBufferSphere, numVerticesSphere);
		}

		for (auto iter = World->GetCubeList().begin(); iter != World->GetCubeList().end();++iter)
		{
			FMatrix Model = JungleMath::CreateModelMatrix((*iter)->GetLocation(), (*iter)->GetRotation(), FVector(0.5f,0.5f,0.5f));
			// 최종 MVP 행렬
			FMatrix MVP = Model * View * Projection;

			renderer.UpdateConstant(MVP);
			renderer.RenderPrimitive(vertexBufferCube, numVerticesCube);
		}


		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// 이후 ImGui UI 컨트롤 추가는 ImGui::NewFrame()과 ImGui::Render() 사이인 여기에 위치합니다.
		ImGui::Begin("Jungle Control Panel");
		ImGui::Text("Hello Jungle World!");
		double fps = 1000.0 / elapsedTime;
		ImGui::Text("FPS %.2f (%.2fms)", fps, elapsedTime);
		ImGui::Separator();
		static int primitiveType = 0;
		const char* primitives[] = { "Sphere", "Cube", "Triangle"};
		ImGui::Combo("Primitive", &primitiveType, primitives, IM_ARRAYSIZE(primitives));

		static int spawnCount = 2;
		ImGui::InputInt("Number of Spawn", &spawnCount, 0, 0);
		if (ImGui::Button("Spawn"))
		{
			World->SpawnObject(static_cast<OBJECTS>(primitiveType));
			spawnCount++;
		}

		static char sceneName[64] = "Default";
		ImGui::InputText("Scene Name", sceneName, IM_ARRAYSIZE(sceneName));

		if (ImGui::Button("New scene")) {
			// Handle new scene creation
		}
		if (ImGui::Button("Save scene")) {
			// Handle saving scene
		}
		if (ImGui::Button("Load scene")) {
			// Handle loading scene
		}

		ImGui::Separator();

		ImGui::Text("Orthogonal");
		ImGui::SliderFloat("FOV", &fov, 30.0f, 120.0f);

		float cameraLocation[3] = { Camera->GetLocation().x, Camera->GetLocation().y, Camera->GetLocation().z};
		ImGui::InputFloat3("Camera Location", cameraLocation);

		float cameraRotation[3] = { Camera->GetRotation().x, Camera->GetRotation().y, Camera->GetRotation().z};
		ImGui::InputFloat3("Camera Rotation", cameraRotation);

		ImGui::End();
		ImGui::Begin("Jungle Property Panel");
		ImGui::Text("Hello Jungle World!");
		UObject* PickObj = World->GetPickingObj();
		if (PickObj) {
			float pickObjLoc[3] = { PickObj->GetLocation().x,PickObj->GetLocation().y ,PickObj->GetLocation().z };
			float pickObjRot[3] = { PickObj->GetRotation().x,PickObj->GetRotation().y ,PickObj->GetRotation().z };
			float pickObjScale[3] = { PickObj->GetScale().x,PickObj->GetScale().y ,PickObj->GetScale().z };

			ImGui::InputFloat3("Tranlsation", pickObjLoc);
			ImGui::InputFloat3("Rotation", pickObjRot);
			ImGui::InputFloat3("Scale", pickObjScale);

			PickObj->SetLocation(FVector(pickObjLoc[0], pickObjLoc[1], pickObjLoc[2]));
			PickObj->SetRotation(FVector(pickObjRot[0], pickObjRot[1], pickObjRot[2]));
			PickObj->SetScale(FVector(pickObjScale[0], pickObjScale[1], pickObjScale[2]));
		}
		ImGui::End();

		// 이후 ImGui UI 컨트롤 추가는 ImGui::NewFrame()과 ImGui::Render() 사이인 여기에 위치합니다.

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
