#pragma once
#include <Windows.h>
#include <d3d11.h>
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_internal.h"
#include "ImGUI/imgui_impl_dx11.h"
#include "ImGUI/imgui_impl_win32.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

class UImGuiManager
{
public:
    void Initialize(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
    void BeginFrame();
    void Render();
    void Shutdown();
};