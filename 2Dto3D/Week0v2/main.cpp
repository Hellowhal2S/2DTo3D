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
#include "GizmoComponent.h"

#include "Sphere.h"
#include "Cube.h"
#include "Gizmo.h"

UGraphicsDevice graphicDevice;
HWND hWnd;
FMatrix View;
FMatrix Projection;
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

	hWnd = CreateWindowExW(0, WindowClass, Title, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
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


	UINT numVerticesGizmo = sizeof(gizmoVertices) / sizeof(FVertexSimple);
	ID3D11Buffer* vertexBufferGizmo = renderer.CreateVertexBuffer(gizmoVertices, sizeof(gizmoVertices));


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

		View = JungleMath::CreateViewMatrix(Camera->GetLocation(), Camera->GetLocation() + Camera->GetForwardVector(), {0, 1, 0});
		Projection = JungleMath::CreateProjectionMatrix(
			fov * (3.141592f / 180.0f),
			1.0f,  // 1:1 비율로 변경
			0.1f,
			1000.0f
		);

		// 준비 작업a
		renderer.Prepare();
		renderer.PrepareShader();


		//Sphere
		for (auto iter = World->GetSphreList().begin(); iter != World->GetSphreList().end();++iter)
		{
			FMatrix Model = JungleMath::CreateModelMatrix((*iter)->GetLocation(), (*iter)->GetRotation(), (*iter)->GetScale());

			// 최종 MVP 행렬
			FMatrix MVP = Model * View * Projection;
			if ((*iter) == World->GetPickingObj()) {
				renderer.UpdateConstant(MVP, 1.0f);
			}
			else
				renderer.UpdateConstant(MVP, 0.0f);

			renderer.RenderPrimitive(vertexBufferSphere, numVerticesSphere);
		}
		//Cube 
		for (auto iter = World->GetCubeList().begin(); iter != World->GetCubeList().end();++iter)
		{
			FMatrix Model = JungleMath::CreateModelMatrix((*iter)->GetLocation(), (*iter)->GetRotation(), FVector(0.5f,0.5f,0.5f));
			// 최종 MVP 행렬
			FMatrix MVP = Model * View * Projection;

			renderer.UpdateConstant(MVP, 0.0f);
			renderer.RenderPrimitive(vertexBufferCube, numVerticesCube);
		}
		// World Gizmo 
		graphicDevice.DeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
		FMatrix Model = JungleMath::CreateModelMatrix(FVector(0.f, 0.f, 0.f),
			FVector(0.f,0.f,0.f), FVector(1000000.0f, 1000000.0f, 1000000.0f));
		FMatrix MVP = Model * View * Projection;
		renderer.UpdateConstant(MVP,0.0f);
		renderer.RenderPrimitive(vertexBufferGizmo, numVerticesGizmo);
		graphicDevice.DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//Local Gizmo
		if (World->GetPickingObj())
		{
			UObject* pickedObj = World->GetPickingObj();
			for (int i = 0;i < 3;i++)
			{
				FVector Location = pickedObj->GetLocation() + World->LocalGizmo[i]->GetLocation();
				if (i == 0)
					Location = Location + pickedObj->GetRightVector();
				else if (i == 1)
					Location = Location + pickedObj->GetUpVector();
				else if (i == 2)
					Location = Location + pickedObj->GetForwardVector();
				FMatrix Model = JungleMath::CreateModelMatrix(Location,
					pickedObj->GetRotation(),
					World->LocalGizmo[i]->GetScale());
				FMatrix MVP = Model * View * Projection;
				if(World->LocalGizmo[i] == World->GetPickingGizmo())
					renderer.UpdateConstant(MVP, 1.0f);
				else 
					renderer.UpdateConstant(MVP, 0.0f);
				renderer.RenderPrimitive(vertexBufferCube, numVerticesCube);
			}
		}
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// 이후 ImGui UI 컨트롤 추가는 ImGui::NewFrame()과 ImGui::Render() 사이인 여기에 위치합니다.
		ImGui::Begin("Jungle Control Panel");
		ImGui::Text("Hello Jungle World!");
		double fps = 1000.0 / elapsedTime;
		ImGui::Text("FPS %.2f (%.2fms)", fps, elapsedTime);
		if(World->GetPickingObj())
			ImGui::Text("%f %.2f %.2f)", World->GetPickingObj()->GetRightVector().x, World->GetPickingObj()->GetRightVector().y, World->GetPickingObj()->GetRightVector().z);
		if (World->GetPickingObj())
			ImGui::Text("%f %.2f %.2f)", World->GetPickingGizmo()->GetScale().x, World->GetPickingGizmo()->GetScale().y, World->GetPickingGizmo()->GetScale().z);
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
