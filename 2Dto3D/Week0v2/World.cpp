#include "World.h"
#include "Cube.h"
#include "Sphere.h"
#include "Arrow.h"

float ndcX;
float ndcY;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
    {
        return true;
    }

    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

UWorld::UWorld() : hWnd(nullptr) {}
UWorld::~UWorld() {}

void UWorld::Initialize(HINSTANCE hInstance)
{
    CreateMainWindow(hInstance);
    InputHandler.Initialize(hInstance, hWnd, 1024, 1024);
    Camera.Initialize();
    Camera.SetProjection(1024.0f / 1024.0f, 0.1f, 1000.0f);
    graphicDevice.Initialize(hWnd);
    renderer.Initialize(&graphicDevice);
    imguiManager.Initialize(hWnd, graphicDevice.Device, graphicDevice.DeviceContext);

    UINT numVerticesSphere = sizeof(sphere_vertices) / sizeof(FVertexSimple);
    vertexBufferSphere = renderer.CreateVertexBuffer(sphere_vertices, sizeof(sphere_vertices));
    UINT numVerticescue = sizeof(cube_vertices) / sizeof(FVertexSimple);
    vertexBufferCube = renderer.CreateVertexBuffer(cube_vertices, sizeof(cube_vertices));

    CreateGizmo(); // 기즈모 생성 추가
}


void UWorld::CreateMainWindow(HINSTANCE hInstance)
{
    const WCHAR WindowClass[] = L"JungleWindowClass";
    const WCHAR Title[] = L"Game Tech Lab";

    WNDCLASSW wndclass = { 0 };
    wndclass.lpfnWndProc = WndProc;
    wndclass.hInstance = hInstance;
    wndclass.lpszClassName = WindowClass;

    RegisterClassW(&wndclass);

    hWnd = CreateWindowExW(0, WindowClass, Title, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1024, 1024, nullptr, nullptr, hInstance, nullptr);
}

void UWorld::Run()
{
    const int targetFPS = 60;
    const double targetFrameTime = 1000.0 / targetFPS;
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

	
    LARGE_INTEGER startTime, endTime;
    double elapsedTime = 1.0;

    while (!bIsExit)
    {
        QueryPerformanceCounter(&startTime);

        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                bIsExit = true;
                break;
            }
        }

        Update();
        Render();

        graphicDevice.SwapBuffer();

        do
        {
            Sleep(0);
            QueryPerformanceCounter(&endTime);
            elapsedTime = (endTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart;
        } while (elapsedTime < targetFrameTime);
    }
}

bool isSelected = false;
bool istoggled = false;


void UWorld::Update()
{
    if (!InputHandler.Frame()) {
        bIsExit = true;
        return;
    }

    //  카메라 이동 (WASD 키 입력)
    if (InputHandler.IsKeyPressed(DIK_W)) {
        Camera.MoveForward(0.1f);
    }
    if (InputHandler.IsKeyPressed(DIK_S)) {
        Camera.MoveForward(-0.1f);
    }
    if (InputHandler.IsKeyPressed(DIK_A)) {
        Camera.MoveRight(-0.1f);
    }
    if (InputHandler.IsKeyPressed(DIK_D)) {
        Camera.MoveRight(0.1f);
    }
    if (InputHandler.IsKeyPressed(DIK_SPACE)) {
        if (!istoggled)
            ToggleGizmoMode(); 
        istoggled = true;
    }
    else
    {
        istoggled = false;
    }

    int mouseX, mouseY;
    InputHandler.GetMouseLocation(mouseX, mouseY);

    if (InputHandler.IsMousePressed())
    {
        if (!isDraggingGizmo)
        {
            SelectObjectWithMouse();
            isDraggingGizmo = true;
            lastMouseX = mouseX;
            lastMouseY = mouseY;
        }

        if (isDraggingGizmo && currentGizmo)
        {
            float deltaX = (mouseX - lastMouseX) * 0.01f;
            float deltaY = (mouseY - lastMouseY) * 0.01f;

            switch (GizmoMode)
            {
            case EGizmoMode::Translate:
                HandleTranslation(deltaX, deltaY);
                break;
            case EGizmoMode::Rotate:
                HandleRotation(deltaX, deltaY);
                break;
            case EGizmoMode::Scale:
                HandleScaling(deltaX, deltaY);
                break;
            }

            lastMouseX = mouseX;
            lastMouseY = mouseY;
        }


    }
    else
    {
        isDraggingGizmo = false; // 마우스 버튼이 해제되면 드래그 종료
        currentGizmo = nullptr;
    }
    UpdateGizmos();

    InputHandler.GetMouseLocation(mouseX, mouseY);

    static bool firstClick = true;
    bool isRightMouseDown = InputHandler.IsRightMousePressed();

    if (isRightMouseDown) {
        if (firstClick) {
            // 우클릭 처음 눌렀을 때 마우스 위치 저장
            lastMouseX = mouseX;
            lastMouseY = mouseY;
            firstClick = false;
        }

        // 마우스 이동량 계산
        float deltaX = (mouseX - lastMouseX) * 0.1f;
        float deltaY = (mouseY - lastMouseY) * 0.1f;

        // 마우스 이동 시 회전 적용
        if (deltaX != 0.0f || deltaY != 0.0f) {
            Camera.Rotate(deltaX, -deltaY);
        }

        // 현재 마우스 위치 업데이트
        lastMouseX = mouseX;
        lastMouseY = mouseY;
    }
    else {
        //  우클릭이 해제되면 firstClick 초기화
        firstClick = true;
    }
    Camera.SetProjection(1024.0f / 1024.0f, 0.1f, 1000.0f);
}


void UWorld::Render()
{
    renderer.Prepare();
    renderer.PrepareShader();

    // 기존 오브젝트 렌더링
    for (auto& Object : ObjectList)
    {
        USceneComponent* sceneComp = dynamic_cast<USceneComponent*>(Object);
        if (!sceneComp) continue;

        FMatrix translationMatrix = FMatrix::CreateTranslationMatrix(sceneComp->RelativeLocation);
        FMatrix rotationMatrix = FMatrix::CreateRotation(sceneComp->RelativeRotation.x,
            sceneComp->RelativeRotation.y,
            sceneComp->RelativeRotation.z);
        FMatrix scaleMatrix = FMatrix::CreateScale(sceneComp->RelativeScale3D.x,
            sceneComp->RelativeScale3D.y,
            sceneComp->RelativeScale3D.z);

        // 변환 순서: S * R * T
        FMatrix worldMatrix = scaleMatrix * rotationMatrix * translationMatrix;

        //M V P
        FMatrix matrix = worldMatrix * Camera.GetViewMatrix() * Camera.GetProjectionMatrix();
        if (UCubeComp* Cube = dynamic_cast<UCubeComp*>(Object))
        {   
            if(currentObject == sceneComp)
                renderer.UpdateConstant(matrix, 4);
            else
                renderer.UpdateConstant(matrix, 0);
            renderer.RenderPrimitive(vertexBufferCube, sizeof(cube_vertices) / sizeof(FVertexSimple));
        }

        if (USphereComp* Sphere = dynamic_cast<USphereComp*>(Object))
        {
            if (currentObject == sceneComp)
                renderer.UpdateConstant(matrix, 4);
            else
                renderer.UpdateConstant(matrix, 0);
            renderer.RenderPrimitive(vertexBufferSphere, sizeof(sphere_vertices) / sizeof(FVertexSimple));
        }
    }

   
    if (currentObject)
    {
        for (size_t i = 0; i < GizmoList.size(); ++i)
        {
            FMatrix translationMatrix = FMatrix::CreateTranslationMatrix(GizmoList[i]->RelativeLocation);
            FMatrix rotationMatrix = FMatrix::CreateRotation(GizmoList[i]->RelativeRotation.x, GizmoList[i]->RelativeRotation.y, GizmoList[i]->RelativeRotation.z);
            FMatrix scaleMatrix = FMatrix::CreateScale(GizmoList[i]->RelativeScale3D.x, GizmoList[i]->RelativeScale3D.y, GizmoList[i]->RelativeScale3D.z);

            FMatrix worldMatrix = scaleMatrix * rotationMatrix * translationMatrix;
            FMatrix matrix = worldMatrix * Camera.GetViewMatrix() * Camera.GetProjectionMatrix();

            if (currentGizmo == GizmoList[i])
            {
                renderer.UpdateConstant(matrix, 0); 
            }
            else
            {
                renderer.UpdateConstant(matrix, i + 1);
            }
            renderer.RenderPrimitive(vertexBufferCube, sizeof(cube_vertices) / sizeof(FVertexSimple));
        }


    }

    // 월드 기즈모 렌더링
    renderer.Graphics->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    FMatrix gizmoWorldMatrix = FMatrix::Identity;
    renderer.UpdateConstant(gizmoWorldMatrix* Camera.GetViewMatrix() * Camera.GetProjectionMatrix(),0);
    renderer.RenderPrimitive(vertexBufferGizmo, 6);

    imguiManager.BeginFrame();
    ImGui::Begin("Jungle Control Panel");
    ImGui::Text("Hello Jungle World");
    ImGui::Text("FPS 60");
    ImGui::Separator();
    // 현재 선택된 오브젝트 타입
    static EObjectType selectedObjectType = EObjectType::Cube;

    // 드롭다운에서 선택할 이름 리스트
    const char* objectTypeNames[] = {"Cube", "Sphere" };

    ImGui::Text("Select Object Type");

    // ImGui 드롭다운에서 Enum 값을 선택할 수 있도록 변환
    int selectedIndex = static_cast<int>(selectedObjectType);
    if (ImGui::Combo("##ObjectType", &selectedIndex, objectTypeNames, IM_ARRAYSIZE(objectTypeNames)))
    {
        selectedObjectType = static_cast<EObjectType>(selectedIndex);
    }

    // "Spawn" 버튼을 눌렀을 때 오브젝트 생성
    if (ImGui::Button("Spawn"))
    {
        USceneComponent* newObject = nullptr;
        switch (selectedObjectType)
        {
        case EObjectType::Cube:
            newObject = new UCubeComp();
            break;
        case EObjectType::Sphere:
            newObject = new USphereComp();
            break;
        default:
            break;
        }

        if (newObject)
        {
            newObject->RelativeLocation = FVector(0.0f, 0.0f, 0.0f); 
            newObject->RelativeRotation = FVector(0.0f, 0.0f, 0.0f);
            newObject->RelativeScale3D = FVector(1.0f, 1.0f, 1.0f); 

            ObjectList.push_back(newObject);
            currentObject = newObject; 
        }
    }
    ImGui::Separator();
    SaveManager.RenderSceneManagerUI(ObjectList);

    ImGui::Checkbox("Orthgraphic", &Camera.bOrthographic);
    ImGui::SliderFloat("##FOV", &Camera.fovDegrees, 0.0f, 180.0f, "%.1f");
    float position[3] = { Camera.RelativeLocation.x, Camera.RelativeLocation.y, Camera.RelativeLocation.z };
    ImGui::PushItemWidth(150);
    ImGui::InputFloat3("Position", position, "%.2f", ImGuiInputTextFlags_ReadOnly);
    float lookAt[3] = { Camera.GetForward().x, Camera.GetForward().y, Camera.GetForward().z };
    ImGui::InputFloat3("LookAt", lookAt, "%.2f", ImGuiInputTextFlags_ReadOnly);
    ImGui::End();

    

    ImGui::Begin("Ray Info");
    ImGui::Text("Ray Origin: (%.2f, %.2f, %.2f)", lastRayOrigin.x, lastRayOrigin.y, lastRayOrigin.z);
    ImGui::Text("Ray Direction: (%.2f, %.2f, %.2f)", lastRayDirection.x, lastRayDirection.y, lastRayDirection.z);
    ImGui::Text("mouse position: %.2f, %.2f", ndcX, ndcY);
    ImGui::End();

  

    if (currentObject)
    {
        ImGui::Begin("123");
        ImGui::Text("Transform Controls");
        ImGui::InputFloat3("Location", (float*)&(currentObject->RelativeLocation));
        ImGui::InputFloat3("Rotation", (float*)&(currentObject->RelativeRotation));
        ImGui::InputFloat3("Scale", (float*)&(currentObject->RelativeScale3D));
        ImGui::End();
    }
	imguiManager.Render();
}   



void UWorld::Release()
{
    renderer.ReleaseVertexBuffer(vertexBufferSphere);
    renderer.ReleaseVertexBuffer(vertexBufferGizmo); // 기즈모 버퍼 해제
    renderer.Release();
    graphicDevice.Release();
    imguiManager.Shutdown();
}


void UWorld::AddObject(UObject* NewObject)
{
    if (NewObject)
    {
        ObjectList.push_back(NewObject);
    }
}

void UWorld::RemoveObject(UObject* ObjectToRemove)
{
    ObjectList.erase(std::remove(ObjectList.begin(), ObjectList.end(), ObjectToRemove), ObjectList.end());
}

void UWorld::SpawnCube()
{
	UCubeComp* NewCube = new UCubeComp();
	NewCube->RelativeLocation = FVector(0.0f, 0.0f, 2.0f);
    NewCube->RelativeScale3D = FVector(1.0f, 1.0f, 1.0f);
	AddObject(NewCube);

}  

void UWorld::SpawnSphere()
{
	USphereComp* NewSphere = new USphereComp();
	AddObject(NewSphere);
}

void UWorld::CreateGizmo()
{
    // 기존 축 기즈모 생성 (X, Y, Z)
    FVertexSimple gizmoVertices[] =
    {
        { 0.0f, 0.0f, 0.0f, 1, 0, 0, 1 }, { 100.0f, 0.0f, 0.0f, 1, 0, 0, 1 }, // X축
        { 0.0f, 0.0f, 0.0f, 0, 1, 0, 1 }, { 0.0f, 100.0f, 0.0f, 0, 1, 0, 1 }, // Y축
        { 0.0f, 0.0f, 0.0f, 0, 0, 1, 1 }, { 0.0f, 0.0f, 100.0f, 0, 0, 1, 1 }  // Z축
    };

    vertexBufferGizmo = renderer.CreateVertexBuffer(gizmoVertices, sizeof(gizmoVertices));

    //  Arrow용 버퍼 생성
    
    //vertexBufferArrow[0] = renderer.CreateVertexBuffer(arrow_x_vertices, sizeof(arrow_x_vertices));
    //vertexBufferArrow[1] = renderer.CreateVertexBuffer(arrow_y_vertices, sizeof(arrow_y_vertices));
    //vertexBufferArrow[2] = renderer.CreateVertexBuffer(arrow_z_vertices, sizeof(arrow_z_vertices));
    for (int i = 0; i < 3; i++)
    {
        UGizmoComp* NewGizmo = new UGizmoComp();
        GizmoList.push_back(NewGizmo);
    }
}




void UWorld::SelectObjectWithMouse()
{
    int mouseX, mouseY;
    InputHandler.GetMouseAbsolutePosition(mouseX, mouseY);

    // NDC 좌표 계산
    ndcX = (2.0f * mouseX) / 1024.0f - 1.0f;
    ndcY = 1.0f - (2.0f * mouseY) / 1024.0f;

    // NDC에서 뷰 공간으로 변환
    FMatrix invProjection = FMatrix::Inverse(Camera.GetProjectionMatrix());
    FVector4 rayClip(ndcX, ndcY, 1.0f, 1.0f);
    FVector4 rayEye = FMatrix::TransformVector(rayClip, invProjection);
    rayEye.z = 1.0f;
    rayEye.a = 0.0f;

    // 뷰 공간에서 월드 공간으로 변환
    FMatrix invView = FMatrix::Inverse(Camera.GetViewMatrix());
    FVector4 rayWorld4 = FMatrix::TransformVector(rayEye, invView);
    FVector rayWorld(rayWorld4.x, rayWorld4.y, rayWorld4.z);
    rayWorld = rayWorld.Normalize();

    // Ray의 원점과 방향 설정
    lastRayOrigin = Camera.RelativeLocation;
    lastRayDirection = rayWorld;

    //  기즈모 선택 검사
    float closestGizmoDistance = FLT_MAX;
    UGizmoComp* selectedGizmo = nullptr;
    for (auto& gizmo : GizmoList)
    {
        float hitDistance;
        if (RayIntersectsObject(lastRayOrigin, lastRayDirection, gizmo, hitDistance))
        {
            if (hitDistance < closestGizmoDistance)
            {
                closestGizmoDistance = hitDistance;
                selectedGizmo = gizmo;
            }
        }
    }

    if (selectedGizmo)
    {
        currentGizmo = selectedGizmo;

    }
    //  기존 객체 선택 검사
    float closestObjectDistance = FLT_MAX;
    USceneComponent* selectedObject = nullptr;
    for (auto& Object : ObjectList)
    {
        USceneComponent* sceneComp = dynamic_cast<USceneComponent*>(Object);
        if (sceneComp)
        {
            float hitDistance;
            if (RayIntersectsObject(lastRayOrigin, lastRayDirection, sceneComp, hitDistance))
            {
                if (hitDistance < closestObjectDistance && hitDistance < closestGizmoDistance)
                {
                    closestObjectDistance = hitDistance;
                    selectedObject = sceneComp;
                }
            }
        }
    }

    if (selectedObject)
    {
        currentObject = selectedObject;
        currentGizmo = nullptr;  // 오브젝트를 선택했으므로 기존 기즈모 선택 해제
    }
}




//OBB로 수정
bool UWorld::RayIntersectsObject(const FVector& rayOrigin, const FVector& rayDir, USceneComponent* obj, float& hitDistance)
{
    // 오브젝트의 월드 변환 행렬 생성 (위치, 회전 적용)
    FMatrix rotationMatrix = FMatrix::CreateRotation(
        obj->RelativeRotation.x,
        obj->RelativeRotation.y,
        obj->RelativeRotation.z
    );

    FMatrix translationMatrix = FMatrix::CreateTranslationMatrix(obj->RelativeLocation);

    // 최종 변환 행렬
    FMatrix worldMatrix = rotationMatrix * translationMatrix;

    // OBB의 로컬 X, Y, Z 축 가져오기
    FVector axisX(worldMatrix.M[0][0], worldMatrix.M[0][1], worldMatrix.M[0][2]);
    FVector axisY(worldMatrix.M[1][0], worldMatrix.M[1][1], worldMatrix.M[1][2]);
    FVector axisZ(worldMatrix.M[2][0], worldMatrix.M[2][1], worldMatrix.M[2][2]);

    // 기즈모인지 확인
    bool isGizmo = (dynamic_cast<UGizmoComp*>(obj) != nullptr);

    // OBB의 반 크기 적용 (기즈모의 경우 Y,Z 스케일을 0.2로 조정)
    FVector halfSize = obj->RelativeScale3D;

    FVector p = obj->RelativeLocation - rayOrigin; // 레이 원점에서 OBB 중심까지의 벡터
    FVector d = rayDir.Normalize(); // 레이 방향 정규화

    float tMin = -FLT_MAX, tMax = FLT_MAX;

    // 각 축(X, Y, Z)에 대해 레이와의 충돌 검사 수행
    FVector axes[3] = { axisX, axisY, axisZ };
    float halfSizes[3] = { halfSize.x, halfSize.y, halfSize.z };

    for (int i = 0; i < 3; i++)
    {
        float e = axes[i].Dot(p);  // 원점에서 타겟까지의 거리를 해당 축으로 투영
        float f = axes[i].Dot(d);  // 레이를 투영

        if (fabs(f) > 1e-6)  // 방향 벡터가 0이면 큰일남
        {
            float t1 = (e - halfSizes[i]) / f;
            float t2 = (e + halfSizes[i]) / f;

            if (t1 > t2) std::swap(t1, t2); // t1이 항상 작은 값이 되도록 설정

            tMin = max(tMin, t1);  // 범위를 계속 줄여나감
            tMax = min(tMax, t2);

            if (tMin > tMax) return false; // 충돌 없음
        }
        else if (-e - halfSizes[i] > 0 || -e + halfSizes[i] < 0)
        {
            return false; // 레이가 OBB 내부에 없고, 지나가지도 않음
        }
    }

    hitDistance = tMin;
    return true;
}


void UWorld::UpdateGizmos()
{
    if (!currentObject) return;

    FVector location = currentObject->RelativeLocation;
    FVector scale = currentObject->RelativeScale3D;
    FVector rotation = currentObject->RelativeRotation;

    if (GizmoList.size() < 3) return;

    // X축 기즈모 (빨강)
    FVector xScale = FVector(max(scale.x * 1.5, 1), 0.2, 0.2);
    GizmoList[0]->SetTransform(location, rotation, xScale);

    // Y축 기즈모 (초록) - X축 Arrow를 Z축 기준으로 90도 회전
    FVector yScale = FVector(0.2, max(1, scale.y * 1.5), 0.2);
    FVector yRotation = FVector(rotation.x, rotation.y, rotation.z);
    GizmoList[1]->SetTransform(location, yRotation, yScale);

    // Z축 기즈모 (파랑) - X축 Arrow를 Y축 기준으로 -90도 회전
    FVector zScale = FVector(0.2, 0.2, max(1, scale.z * 1.5));
    FVector zRotation = FVector(rotation.x, rotation.y, rotation.z);
    GizmoList[2]->SetTransform(location, zRotation, zScale);
}

void UWorld::ToggleGizmoMode()
{
    switch (GizmoMode)
    {
    case EGizmoMode::Translate:
        GizmoMode = EGizmoMode::Rotate;
        break;
    case EGizmoMode::Rotate:
        GizmoMode = EGizmoMode::Scale;
        break;
    case EGizmoMode::Scale:
        GizmoMode = EGizmoMode::Translate;
        break;
    }
}


void UWorld::HandleTranslation(float deltaX, float deltaY)
{
    FVector moveDirection = FVector(0, 0, 0);

    // 기즈모의 로컬 이동 방향 설정 (X, Y, Z 축)
    FVector localMoveDirection(0, 0, 0);
    if (currentGizmo == GizmoList[0]) localMoveDirection = FVector(1, 0, 0);
    else if (currentGizmo == GizmoList[1]) localMoveDirection = FVector(0, 1, 0);
    else if (currentGizmo == GizmoList[2]) localMoveDirection = FVector(0, 0, 1);

    // 기즈모의 로컬 회전 행렬 가져오기
    FMatrix gizmoRotationMatrix = FMatrix::CreateRotation(
        currentGizmo->RelativeRotation.x,
        currentGizmo->RelativeRotation.y,
        currentGizmo->RelativeRotation.z
    );

    // 로컬 이동 벡터를 월드 공간으로 변환
    FVector worldMoveDirection = FMatrix::TransformVector(localMoveDirection, gizmoRotationMatrix);
    worldMoveDirection = worldMoveDirection.Normalize();

    // 카메라 뷰 행렬 가져오기
    FMatrix viewMatrix = Camera.GetViewMatrix();

    // 기즈모 이동 방향을 뷰 공간으로 변환
    FVector viewMoveDirection = FMatrix::TransformVector(worldMoveDirection, viewMatrix);
    viewMoveDirection = viewMoveDirection.Normalize();

    // 마우스 이동 벡터 (스크린 공간 기준, 정규화)
    FVector mouseMoveDirection(deltaX, -deltaY, 0);
    mouseMoveDirection = mouseMoveDirection.Normalize();

    // 마우스 이동 벡터와 기즈모 이동 벡터의 내적을 이용한 방향 보정
    float alignment = mouseMoveDirection.Dot(FVector(viewMoveDirection.x, viewMoveDirection.y, 0));
    float sign = (alignment < 0) ? -1.0f : 1.0f; // 방향 반전

    // 보정된 이동 적용
    FVector moveAmount = worldMoveDirection * (sign * sqrt(deltaX * deltaX + deltaY * deltaY));
    currentObject->RelativeLocation = currentObject->RelativeLocation + moveAmount;
}


void UWorld::HandleRotation(float deltaX, float deltaY)
{
    FVector rotationChange = FVector(0, 0, 0);

    if (currentGizmo == GizmoList[0]) rotationChange = FVector(deltaX * 10.0f, 0, 0);
    else if (currentGizmo == GizmoList[1]) rotationChange = FVector(0, deltaX * 10.0f, 0);
    else if (currentGizmo == GizmoList[2]) rotationChange = FVector(0, 0, deltaY * 10.0f);

    currentObject->RelativeRotation = currentObject->RelativeRotation + rotationChange;
}

void UWorld::HandleScaling(float deltaX, float deltaY)
{
    FVector scaleChange = FVector(1, 1, 1);

    if (currentGizmo == GizmoList[0]) scaleChange = FVector(1.0f + deltaX, 1, 1);
    else if (currentGizmo == GizmoList[1]) scaleChange = FVector(1, 1.0f + deltaX, 1);
    else if (currentGizmo == GizmoList[2]) scaleChange = FVector(1, 1, 1.0f - deltaY);

    currentObject->RelativeScale3D.x *= scaleChange.x;
    currentObject->RelativeScale3D.y *= scaleChange.y;
    currentObject->RelativeScale3D.z *= scaleChange.z;
}






