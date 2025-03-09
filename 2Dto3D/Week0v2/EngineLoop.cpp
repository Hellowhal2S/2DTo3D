#include "EngineLoop.h"
#include "ImGuiManager.h"
#include "World.h"
#include "CameraComponent.h"
#include "JungleMath.h"
#include "ControlPaner.h"
#include "PropertyPanel.h"
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
	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED) {
			//UGraphicsDevice 객체의 OnResize 함수 호출
			if (FEngineLoop::graphicDevice.SwapChain) {
				FEngineLoop::graphicDevice.OnResize(hWnd);
			}
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

FGraphicsDevice FEngineLoop::graphicDevice;
FRenderer FEngineLoop::renderer;
FResourceMgr FEngineLoop::resourceMgr;

FEngineLoop::FEngineLoop()
{
}

FEngineLoop::~FEngineLoop()
{
}

int32 FEngineLoop::PreInit()
{
	return 0;
}

int32 FEngineLoop::Init(HINSTANCE hInstance)
{
	WindowInit(hInstance);
	graphicDevice.Initialize(hWnd);
	renderer.Initialize(&graphicDevice);
	UIMgr = new UImGuiManager;
	UIMgr->Initialize(hWnd,graphicDevice.Device, graphicDevice.DeviceContext);
	resourceMgr.Initialize(&renderer);
	GWorld = new UWorld;
	GWorld->Initialize();

	return 0;
}



void FEngineLoop::Tick()
{
	LARGE_INTEGER frequency;
	const double targetFrameTime = 1000.0 / targetFPS; // 한 프레임의 목표 시간 (밀리초 단위)

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
		GWorld->Tick(elapsedTime);

		UCameraComponent* Camera = static_cast<UCameraComponent*>(GWorld->GetCamera());
		View = JungleMath::CreateViewMatrix(Camera->GetLocation(), Camera->GetLocation() + Camera->GetForwardVector(), { 0, 1, 0 });
		Projection = JungleMath::CreateProjectionMatrix(
			Camera->GetFov() * (3.141592f / 180.0f),
			1.0f,  // 1:1 비율로 변경
			0.1f,
			1000.0f
		);

		graphicDevice.Prepare();
		renderer.PrepareShader();

		Render();
#pragma region MyRegion
		////Sphere
//for (auto iter = GWorld->GetSphreList().begin(); iter != GWorld->GetSphreList().end();++iter)
//{
//	FMatrix Model = JungleMath::CreateModelMatrix((*iter)->GetLocation(), (*iter)->GetRotation(), (*iter)->GetScale());

//	// 최종 MVP 행렬
//	FMatrix MVP = Model * View * Projection;
//	if ((*iter) == GWorld->GetPickingObj()) {
//		renderer.UpdateConstant(MVP, 1.0f);
//	}
//	else
//		renderer.UpdateConstant(MVP, 0.0f);

//	renderer.RenderPrimitive(resourceMgr.vertexBufferSphere, resourceMgr.numVerticesSphere);
//}
////Cube 
//for (auto iter = GWorld->GetCubeList().begin(); iter != GWorld->GetCubeList().end();++iter)
//{
//	FMatrix Model = JungleMath::CreateModelMatrix((*iter)->GetLocation(), (*iter)->GetRotation(), FVector(0.5f, 0.5f, 0.5f));
//	// 최종 MVP 행렬
//	FMatrix MVP = Model * View * Projection;

//	renderer.UpdateConstant(MVP, 0.0f);
//	renderer.RenderPrimitive(resourceMgr.vertexBufferCube, resourceMgr.numVerticesSphere);
//}
//// World Gizmo 
//graphicDevice.DeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
//FMatrix Model = JungleMath::CreateModelMatrix(FVector(0.f, 0.f, 0.f),
//	FVector(0.f, 0.f, 0.f), FVector(1000000.0f, 1000000.0f, 1000000.0f));
//FMatrix MVP = Model * View * Projection;
//renderer.UpdateConstant(MVP, 0.0f);
//renderer.RenderPrimitive(resourceMgr.vertexBufferGizmo, resourceMgr.numVerticesGizmo);
//graphicDevice.DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

////Local Gizmo
//if (GWorld->GetPickingObj())
//{
//	UObject* pickedObj = GWorld->GetPickingObj();
//	for (int32 i = 0;i < 3;i++)
//	{
//		FVector Location = pickedObj->GetLocation() +GWorld->LocalGizmo[i]->GetLocation();
//		if (i == 0)
//			Location = Location + pickedObj->GetRightVector();
//		else if (i == 1)
//			Location = Location + pickedObj->GetUpVector();
//		else if (i == 2)
//			Location = Location + pickedObj->GetForwardVector();
//		FMatrix Model = JungleMath::CreateModelMatrix(Location,
//			pickedObj->GetRotation(),
//			GWorld->LocalGizmo[i]->GetScale());
//		FMatrix MVP = Model * View * Projection;
//		if (GWorld->LocalGizmo[i] == GWorld->GetPickingGizmo())
//			renderer.UpdateConstant(MVP, 1.0f);
//		else
//			renderer.UpdateConstant(MVP, 0.0f);
//		renderer.RenderPrimitive(resourceMgr.vertexBufferCube, resourceMgr.numVerticesSphere);
//	}
//}
#pragma endregion

		UIMgr->BeginFrame();

		Console::GetInstance().Draw();
		ControlPanel::GetInstance().Draw(GetWorld(),elapsedTime);
		PropertyPanel::GetInstance().Draw(GetWorld());
#pragma region MyRegion
		//
		//ImGui::Begin("Jungle Control Panel");
		//ImGui::Text("Hello Jungle World!");
		//double fps = 1000.0 / elapsedTime;
		//ImGui::Text("FPS %.2f (%.2fms)", fps, elapsedTime);

		//ImGui::Separator();
		//static int32 primitiveType = 0;
		//const char* primitives[] = { "Sphere", "Cube", "Triangle" };
		//ImGui::Combo("Primitive", &primitiveType, primitives, IM_ARRAYSIZE(primitives));

		//static int32 spawnCount = 2;
		//ImGui::InputInt("Number of Spawn", &spawnCount, 0, 0);
		//if (ImGui::Button("Spawn"))
		//{
		//	GWorld->SpawnObject(static_cast<OBJECTS>(primitiveType));
		//	spawnCount++;
		//}

		//static char sceneName[64] = "Default";
		//ImGui::InputText("Scene Name", sceneName, IM_ARRAYSIZE(sceneName));

		//if (ImGui::Button("New scene")) {
		//	// Handle new scene creation
		//}
		//if (ImGui::Button("Save scene")) {
		//	// Handle saving scene
		//}
		//if (ImGui::Button("Load scene")) {
		//	// Handle loading scene
		//}

		//ImGui::Separator();

		//ImGui::Text("Orthogonal");
		//ImGui::SliderFloat("FOV", &fov, 30.0f, 120.0f);

		//float cameraLocation[3] = { Camera->GetLocation().x, Camera->GetLocation().y, Camera->GetLocation().z };
		//ImGui::InputFloat3("Camera Location", cameraLocation);

		//float cameraRotation[3] = { Camera->GetRotation().x, Camera->GetRotation().y, Camera->GetRotation().z };
		//ImGui::InputFloat3("Camera Rotation", cameraRotation);

		//Camera->SetLocation(FVector(cameraLocation[0], cameraLocation[1], cameraLocation[2]));
		//Camera->SetRotation(FVector(cameraRotation[0], cameraRotation[1], cameraRotation[2]));

		//ImGui::End();
		//
#pragma endregion
#pragma region MyRegion
		//ImGui::Begin("Jungle Property Panel");
		//ImGui::Text("Hello Jungle World!");
		//UObject* PickObj = GWorld->GetPickingObj();
		//if (PickObj) {
		//	float pickObjLoc[3] = { PickObj->GetLocation().x,PickObj->GetLocation().y ,PickObj->GetLocation().z };
		//	float pickObjRot[3] = { PickObj->GetRotation().x,PickObj->GetRotation().y ,PickObj->GetRotation().z };
		//	float pickObjScale[3] = { PickObj->GetScale().x,PickObj->GetScale().y ,PickObj->GetScale().z };

		//	ImGui::InputFloat3("Tranlsation", pickObjLoc);
		//	ImGui::InputFloat3("Rotation", pickObjRot);
		//	ImGui::InputFloat3("Scale", pickObjScale);

		//	PickObj->SetLocation(FVector(pickObjLoc[0], pickObjLoc[1], pickObjLoc[2]));
		//	PickObj->SetRotation(FVector(pickObjRot[0], pickObjRot[1], pickObjRot[2]));
		//	PickObj->SetScale(FVector(pickObjScale[0], pickObjScale[1], pickObjScale[2]));
		//}
		//ImGui::End();
#pragma endregion
		UIMgr->EndFrame();

		graphicDevice.SwapBuffer();
		do
		{
			Sleep(0);
			QueryPerformanceCounter(&endTime);
			elapsedTime = (endTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart;
		} while (elapsedTime < targetFrameTime);
	}
}

void FEngineLoop::Render()
{
	for (auto iter : GetWorld()->GetObjectArr())
	{
		iter->Render();
	}
}

void FEngineLoop::Exit()
{
	GWorld->Release();
	delete GWorld;
	resourceMgr.Release(&renderer);
	renderer.Release();
	graphicDevice.Release();
	UIMgr->Shutdown();
	delete UIMgr;
}

void FEngineLoop::WindowInit(HINSTANCE hInstance)
{
	WCHAR WindowClass[] = L"JungleWindowClass";

	WCHAR Title[] = L"Game Tech Lab";

	WNDCLASSW wndclass = { 0 };
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.lpszClassName = WindowClass;

	RegisterClassW(&wndclass);

	hWnd = CreateWindowExW(0, WindowClass, Title, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1500, 1500,
		nullptr, nullptr, hInstance, nullptr);
}

