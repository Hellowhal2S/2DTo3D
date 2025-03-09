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
    std::vector<UObject*> ObjectList; // 객체 리스트 관리
    UGraphicsDevice graphicDevice;
    URenderer renderer;
    UImGuiManager imguiManager;
    ID3D11Buffer* vertexBufferSphere = nullptr;
	ID3D11Buffer* vertexBufferCube = nullptr;
    ID3D11Buffer* vertexBufferGizmo = nullptr;
    InputClass InputHandler; // InputClass 추가
    UCamera Camera;
    USceneComponent* currentObject;
    EObjectType Type;
    bool bIsExit = false;

private:
    FVector lastRayOrigin;
    FVector lastRayDirection;

};

