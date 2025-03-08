#include "EngineLoop.h"

//void EngineLoop::InitRenderer(URenderer& renderer) {
//	vertexBufferSphere = renderer.CreateVertexBuffer(sphere_vertices, sizeof(sphere_vertices));
//}
bool EngineLoop::bIsExit;
MSG EngineLoop::msg;
TArray<UWorld*> EngineLoop::WorldList;
ID3D11Buffer* EngineLoop::vertexBufferSphere;
void EngineLoop::InitEngineLoop(URenderer& renderer)
{ 
	EngineLoop::bIsExit = false;
	EngineLoop::vertexBufferSphere = renderer.CreateVertexBuffer(sphere_vertices, sphere_vertices_size);
	
	UWorld* world = new UWorld;
	WorldList.push_back(world);

	WorldList.front()->InitWorld();
}

void EngineLoop:: ProcessInput() {
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg); // Ű���� �Է� �޽����� ���ڸ޽����� ����
		DispatchMessage(&msg); // �޽����� WndProc�� ����

		if (msg.message == WM_QUIT)
		{
			EngineLoop::bIsExit = true;
			break;
		}
		else {
			
		}
		/* ���� InputManager�� ���� ��ǲ ���� �߰�*/
	}
}

void EngineLoop::Update() {

	WorldList.front()->UpdateWorld(0.0f); 
}

void EngineLoop::Render(URenderer& renderer) {
	/* ���� ������ ���� �߰�*/
	renderer.Prepare();
	renderer.PrepareShader();

	WorldList.front()->RenderWorld();
	UINT numVerticesSphere = sphere_vertices_size / sizeof(FVertexSimple);
	renderer.RenderPrimitive(vertexBufferSphere, numVerticesSphere);
	ImGuiManager::RenderImGui();

	
	/* ���� ������ ���� �߰�*/
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