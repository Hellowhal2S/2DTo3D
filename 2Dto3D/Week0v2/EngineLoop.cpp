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
		TranslateMessage(&msg); // 키보드 입력 메시지를 문자메시지로 변경
		DispatchMessage(&msg); // 메시지를 WndProc에 전달

		if (msg.message == WM_QUIT)
		{
			EngineLoop::bIsExit = true;
			break;
		}

		/* 추후 InputManager를 통한 인풋 로직 추가*/
	}
}

void EngineLoop::Update() {
	/* 추후 연산 로직 추가 */
}

void EngineLoop::Render(URenderer& renderer) {
	/* 추후 렌더링 로직 추가*/
	renderer.Prepare();
	renderer.PrepareShader();

	//FIXME : 테스트용 코드
	FVector pos = { 0,  0, 0 };
	float rot = 0;
	float scale = 1;
	renderer.UpdateConstant(pos, scale, rot);

	UINT numVerticesSphere = sphere_vertices_size / sizeof(FVertexSimple);
	renderer.RenderPrimitive(vertexBufferSphere, numVerticesSphere);
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