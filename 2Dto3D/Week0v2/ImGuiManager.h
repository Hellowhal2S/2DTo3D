#pragma once

class UImGuiManager
{
public:
    void Initialize(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
    void BeginFrame();
    void Render();
    void Shutdown();
};

