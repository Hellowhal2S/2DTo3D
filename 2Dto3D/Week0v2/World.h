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
#include <vector> // STL ���� ���

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
    std::vector<UObject*> ObjectList; // ��ü ����Ʈ ����
    UGraphicsDevice graphicDevice;
    URenderer renderer;
    UImGuiManager imguiManager;
    ID3D11Buffer* vertexBufferSphere = nullptr;
    InputClass InputHandler; // InputClass �߰�
    UCamera Camera;
    bool bIsExit = false;
};

