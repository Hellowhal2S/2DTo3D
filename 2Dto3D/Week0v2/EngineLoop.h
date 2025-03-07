#pragma once
#include "windows.h"
#include "Renderer.h"
class EngineLoop {
public:
	static bool bIsExit;
	static MSG msg;
public:
	static void InitEngineLoop() {bIsExit = false;}
	static void Run(URenderer& renderer);

	
private:
	static void ProcessInput();
	static void Update();
	static void Render(URenderer& renderer);
};
