#pragma once
#include "windows.h"
#include "Renderer.h"
#include "ImGuiManager.h"
#include "Sphere.h"
#include "UObject.h"
#include "UCameraComponent.h"
//#include "UWorld.h"
//MOD
class UWorld;
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
	static TArray<UWorld*> WorldList;
	static void ProcessInput();
	static void Update();
	static void Render(URenderer& renderer);

/* 나중에 위치 변경할 임시 변수들*/
public:
	
	
};
