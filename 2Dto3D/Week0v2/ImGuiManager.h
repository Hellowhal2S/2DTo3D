#pragma once
#include "Renderer.h"
class ImGuiManager {
public:
	static void InitImGui(URenderer& renderer, HWND hwnd);
	static void NewImGuiFrame();
	static void DrawRenderData();
	static void SetupUI();
	static void RenderImGui();
	static void ReleaseImGUi();
};