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

enum EGizmoMode {
    Translate,
    Rotate,
    Scale
};

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
    void UpdateGizmos();
    void ToggleGizmoMode(); // 기즈모 모드 변경 함수 추가
    void HandleTranslation(float deltaX, float deltaY);
    void HandleRotation(float deltaX, float deltaY);
    void HandleScaling(float deltaX, float deltaY);


private:
    void CreateMainWindow(HINSTANCE hInstance);
    void Update();
    void Render();


private:
    HWND hWnd;
    std::vector<UObject*> ObjectList;
    std::vector<UGizmoComp*> GizmoList;
    UGraphicsDevice graphicDevice;
    URenderer renderer;
    UImGuiManager imguiManager;
    ID3D11Buffer* vertexBufferSphere = nullptr;
    ID3D11Buffer* vertexBufferCube = nullptr;
    ID3D11Buffer* vertexBufferGizmo = nullptr;
    ID3D11Buffer* vertexBufferArrow[3];
    InputClass InputHandler;
    UCamera Camera;
    USceneComponent* currentObject;
    UGizmoComp* currentGizmo = nullptr;
    USaveManager SaveManager; 
    EObjectType Type;
    bool bIsExit = false;

    EGizmoMode GizmoMode = EGizmoMode::Translate; // 기본값: 이동 모드


    FVector lastRayOrigin;
    FVector lastRayDirection;
    bool isDraggingGizmo = false;
    int lastMouseX = 0, lastMouseY = 0;

};