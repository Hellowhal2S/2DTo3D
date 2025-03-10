#pragma once
#include <Windows.h>

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_internal.h"
#include "ImGUI/imgui_impl_dx11.h"
#include "ImGUI/imgui_impl_win32.h"

class UImGuiManager
{
public:
    void Initialize(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
    void BeginFrame();
    void EndFrame();
    void Shutdown();
};

