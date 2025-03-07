#pragma once
#include "Renderer.h"
class ImGuiManager {
public:
	static void InitImGui(URenderer renderer, HWND hwnd);
	static void NewImGuiFrame();
	static void RenderImGui();
};