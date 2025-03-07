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

	UINT numVerticesCube = sizeof(cube_vertices) / sizeof(FVertexSimple);
	ID3D11Buffer* vertexBufferCube = renderer.CreateVertexBuffer(cube_vertices, sizeof(cube_vertices));


	UWorld* World = new UWorld;
	World->Initialize();

	bool bIsExit = false;

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
		}

		World->Update(elapsedTime);


		// �غ� �۾�a
		renderer.Prepare();
		renderer.PrepareShader();

		UCameraComponent* Camera = static_cast<UCameraComponent*>(World->GetCamera());
		for (auto iter = World->GetSphreList().begin(); iter != World->GetSphreList().end();++iter)
		{
			FMatrix Model = JungleMath::CreateModelMatrix((*iter)->GetLocation(), (*iter)->GetRotation(), (*iter)->GetScale());
			FMatrix View = JungleMath::CreateViewMatrix(Camera->GetLocation(), Camera->GetLocation() + Camera->GetForwardVector(), { 0, 1, 0 });
			FMatrix Projection = JungleMath::CreateProjectionMatrix(45.0f * (3.141592f / 180.0f), 16.0f / 9.0f, 0.1f, 1000.0f);
			Projection = JungleMath::CreateProjectionMatrix(
				45.0f * (3.141592f / 180.0f),
				1.0f,  // 1:1 ������ ����
				0.1f,
				1000.0f
			);
			// ���� MVP ���
			FMatrix MVP = Model * View * Projection;

			renderer.UpdateConstant(MVP);
			renderer.RenderPrimitive(vertexBufferSphere, numVerticesSphere);
		}

		for (auto iter = World->GetCubeList().begin(); iter != World->GetCubeList().end();++iter)
		{
			FMatrix Model = JungleMath::CreateModelMatrix((*iter)->GetLocation(), (*iter)->GetRotation(), (*iter)->GetScale());
			FMatrix View = JungleMath::CreateViewMatrix(Camera->GetLocation(), Camera->GetLocation() + Camera->GetForwardVector(), { 0, 1, 0 });
			FMatrix Projection = JungleMath::CreateProjectionMatrix(45.0f * (3.141592f / 180.0f), 16.0f / 9.0f, 0.1f, 1000.0f);
			Projection = JungleMath::CreateProjectionMatrix(
				45.0f * (3.141592f / 180.0f),
				1.0f,  // 1:1 ������ ����
				0.1f,
				1000.0f
			);
			// ���� MVP ���
			FMatrix MVP = Model * View * Projection;

			renderer.UpdateConstant(MVP);
			renderer.RenderPrimitive(vertexBufferCube, numVerticesCube);
		}


		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// ���� ImGui UI ��Ʈ�� �߰��� ImGui::NewFrame()�� ImGui::Render() ������ ���⿡ ��ġ�մϴ�.
		ImGui::Begin("Console");


		ImGui::Text("Camera Pos: {x:%f, y:%f, z:%f}", World->GetCamera()->GetLocation().x,
			World->GetCamera()->GetLocation().y,
			World->GetCamera()->GetLocation().z);
		ImGui::Text("Camera Rotation: {x:%f, y:%f, z:%f}", World->GetCamera()->GetRotation().x,
			World->GetCamera()->GetRotation().y,
			World->GetCamera()->GetRotation().z
		);
		ImGui::Text("Camera Forward: {x:%f, y:%f, z:%f}",Camera->GetForwardVector().x,
			Camera->GetForwardVector().y,
			 Camera->GetForwardVector().z
		);
		ImGui::End();
		/////////////////////////////////////////////////////////////////////////
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		//���� ��ü
		graphicDevice.SwapBuffer();
		do
		{
			Sleep(0);

			// ���� ���� �ð� ���
			QueryPerformanceCounter(&endTime);

			// �� �������� �ҿ�� �ð� ��� (�и��� ������ ��ȯ)
			elapsedTime = (endTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart;

		} while (elapsedTime < targetFrameTime);
	}


	delete World;
	

	renderer.ReleaseVertexBuffer(vertexBufferSphere);
	renderer.Release();

	graphicDevice.Release();
	// ���⿡�� ImGui �Ҹ�
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	return 0;
}
