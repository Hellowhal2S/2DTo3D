#include "EngineLoop.h"

//void EngineLoop::InitRenderer(URenderer& renderer) {
//	vertexBufferSphere = renderer.CreateVertexBuffer(sphere_vertices, sizeof(sphere_vertices));
//}
bool EngineLoop::bIsExit;
MSG EngineLoop::msg;
TArray<UWorld*> EngineLoop::WorldList;
ID3D11Buffer* EngineLoop::vertexBufferSphere;
int EngineLoop::numVerticesSphere;
HWND EngineLoop::hWnd;
void EngineLoop::InitEngineLoop(URenderer& renderer, HWND hwnd)
{ 
	EngineLoop::bIsExit = false;
	EngineLoop::vertexBufferSphere = renderer.CreateVertexBuffer(sphere_vertices, sphere_vertices_size);
	EngineLoop::hWnd = hwnd;
	UWorld* world = new UWorld;
	WorldList.push_back(world);

	WorldList.front()->InitWorld();
	numVerticesSphere = sphere_vertices_size / sizeof(FVertexSimple);
}

void EngineLoop:: ProcessInput() {
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg); // 키보드 입력 메시지를 문자메시지로 변경
		DispatchMessage(&msg); // 메시지를 WndProc에 전달

		if (msg.message == WM_QUIT)
		{
			EngineLoop::bIsExit = true;
			break;
		}
		else {
			
		}
		/* 추후 InputManager를 통한 인풋 로직 추가*/
	}
}

void EngineLoop::Update() {

	WorldList.front()->UpdateWorld(0.0f); 
	
}

void EngineLoop::Render(URenderer& renderer) {
	FMatrix Model;
	/* 추후 렌더링 로직 추가*/
	renderer.Prepare();
	renderer.PrepareShader();

	UWorld* myWorld = WorldList.front();
	myWorld->RenderWorld();

	for (auto pmv : myWorld->GetPrimitiveList()) {
		UPrimitiveComponent* pmv_comp = static_cast<UPrimitiveComponent*>(pmv);
		Model = FMatrix::GetModelMatrix(pmv_comp->RelativeLocation, pmv_comp->RelativeRotation, pmv_comp->RelativeScale3D);
		//FMatrix MVP = myWorld->mainCamera->GetProjectionMatrix() * myWorld->mainCamera->GetViewMatrix() * Model;

		FMatrix MVP = Model * myWorld->mainCamera->GetViewMatrix() * myWorld->mainCamera->GetProjectionMatrix();

		renderer.UpdateConstant(MVP);

		renderer.RenderPrimitive(vertexBufferSphere, numVerticesSphere);
	
	}
	/*UINT numVerticesSphere = sphere_vertices_size / sizeof(FVertexSimple);
	renderer.RenderPrimitive(vertexBufferSphere, numVerticesSphere);*/
	ImGuiManager::RenderImGui();

	
	/* 추후 렌더링 로직 추가*/
	renderer.SwapBuffer();

}

void EngineLoop::Run(URenderer& renderer)
{
	ProcessInput();
	Update();
	Render(renderer);

}

void EngineLoop::ReleaseVertexBuffer() {
	vertexBufferSphere->Release();
}