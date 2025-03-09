#pragma once
#include <Windows.h>
#include "Define.h"
#include "GraphicDevice.h"
#include "Renderer.h"
#include "ImguiManager.h"
#include "Save.h"
#include "Input.h"
#include "Camera.h"
#include "Model.h"


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
    void SpawnCube();
    void SpawnSphere();
    void CreateGizmo();
    void SelectObjectWithMouse();
    bool RayIntersectsObject(const FVector& rayOrigin, const FVector& rayDir, USceneComponent* obj, float& hitDistance);

private:
    void CreateMainWindow(HINSTANCE hInstance);
    void Update();
    void Render();

private:
    HWND hWnd;
    std::vector<UObject*> ObjectList;
    UGraphicsDevice graphicDevice;
    URenderer renderer;
    UImGuiManager imguiManager;
    ID3D11Buffer* vertexBufferSphere = nullptr;
    ID3D11Buffer* vertexBufferCube = nullptr;
    ID3D11Buffer* vertexBufferGizmo = nullptr;
    ID3D11Buffer* vertexBufferArrow = nullptr;
    InputClass InputHandler;
    UCamera Camera;
    USceneComponent* currentObject;
    USaveManager SaveManager; // 멤버 변수로 이동
    EObjectType Type;

    bool bIsExit = false;

private:
    FVector lastRayOrigin;
    FVector lastRayDirection;
};