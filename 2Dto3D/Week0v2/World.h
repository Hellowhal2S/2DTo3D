#pragma once
#include <Windows.h>
#include "Define.h"
#include "GraphicDevice.h"
#include "Renderer.h"
#include "ImguiManager.h"
#include "Object.h"
#include "Input.h"
#include "Camera.h"
#include "Model.h"
#include <vector> // STL 벡터 사용

class UWorld
{
public:
    UWorld();
    ~UWorld();

    void Initialize(HINSTANCE hInstance);
    void Run();
    void Release();

    void AddObject(UObject* NewObject);
    void RemoveObject(UObject* ObjectToRemove);

private:
    void CreateMainWindow(HINSTANCE hInstance);
    void Update();
    void Render();

private:
    HWND hWnd;
    std::vector<UObject*> ObjectList; // 객체 리스트 관리
    UGraphicsDevice graphicDevice;
    URenderer renderer;
    UImGuiManager imguiManager;
    ID3D11Buffer* vertexBufferSphere = nullptr;
    InputClass InputHandler; // InputClass 추가
    UCamera Camera;
    bool bIsExit = false;
};

