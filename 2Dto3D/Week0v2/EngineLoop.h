#pragma once

#include <Windows.h>
#include <fstream>

#include "Define.h"

#include "GraphicDevice.h"
#include "Renderer.h"
#include "ResourceManager.h"

class UWorld;
class UImGuiManager;

class FEngineLoop
{
public:
	FEngineLoop();
	~FEngineLoop();

	int32 PreInit();
	int32 Init(HINSTANCE hInstance);
	void Tick();
	void Exit();

private:
	void WindowInit(HINSTANCE hInstance, int screenWidth, int screenHeight);
	void LoadEngineConfig(int& screenWidth, int& screenHeight, bool& fullScreen);
	void Render();

public:
	static FGraphicsDevice graphicDevice;
	static FRenderer renderer;
	static FResourceManager resourceManager;
	HWND hWnd;
	FMatrix View;
	FMatrix Projection;
	float aspectRatio = 1.0f;

private:
	UImGuiManager* UIManager;
	UWorld* GWorld;
	bool bIsExit = false;
	const int32 targetFPS = 60;

public:
	UWorld* GetWorld() { return GWorld; }
};
