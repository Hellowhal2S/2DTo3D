#pragma once
#include "windows.h"
#include "Renderer.h"
#include "Renderer.h"
#include "ImGuiManager.h"
#include "Sphere.h"
class EngineLoop {
public:
	static bool bIsExit;
	static MSG msg;
public:
	static void ReleaseVertexBuffer();
	static void InitEngineLoop(URenderer& renderer);
	static void Run(URenderer& renderer);
	//static void InitRenderer(URenderer& renderer);
	static ID3D11Buffer* vertexBufferSphere;
	
private:
	static void ProcessInput();
	static void Update();
	static void Render(URenderer& renderer);

	
	
};
