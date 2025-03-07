#include "EngineLoop.h"

bool EngineLoop::bIsExit = false;
MSG EngineLoop::msg = {};
ID3D11Buffer* EngineLoop::vertexBufferSphere = nullptr;

//void EngineLoop::InitRenderer(URenderer& renderer) {
//	vertexBufferSphere = renderer.CreateVertexBuffer(sphere_vertices, sizeof(sphere_vertices));
//}
void EngineLoop::InitEngineLoop(URenderer& renderer)
{ 
	EngineLoop::bIsExit = false;
	EngineLoop::vertexBufferSphere = renderer.CreateVertexBuffer(sphere_vertices, sphere_vertices_size);
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

		/* ���� InputManager�� ���� ��ǲ ���� �߰�*/
	}
}

void EngineLoop::Update() {
	/* ���� ���� ���� �߰� */
}

void EngineLoop::Render(URenderer& renderer) {
	/* ���� ������ ���� �߰�*/
	renderer.Prepare();
	renderer.PrepareShader();

	//FIXME : �׽�Ʈ�� �ڵ�
	FVector pos = { 0,  0, 0 };
	float rot = 0;
	float scale = 1;
	renderer.UpdateConstant(pos, scale, rot);

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