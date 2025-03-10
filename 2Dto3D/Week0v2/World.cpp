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

    CreateGizmo(); // ����� ���� �߰�
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

    //  ī�޶� �̵� (WASD Ű �Է�)
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
        isDraggingGizmo = false; // ���콺 ��ư�� �����Ǹ� �巡�� ����
        currentGizmo = nullptr;
    }
    UpdateGizmos();

    InputHandler.GetMouseLocation(mouseX, mouseY);

    static bool firstClick = true;
    bool isRightMouseDown = InputHandler.IsRightMousePressed();

    if (isRightMouseDown) {
        if (firstClick) {
            // ��Ŭ�� ó�� ������ �� ���콺 ��ġ ����
            lastMouseX = mouseX;
            lastMouseY = mouseY;
            firstClick = false;
        }

        // ���콺 �̵��� ���
        float deltaX = (mouseX - lastMouseX) * 0.1f;
        float deltaY = (mouseY - lastMouseY) * 0.1f;

        // ���콺 �̵� �� ȸ�� ����
        if (deltaX != 0.0f || deltaY != 0.0f) {
            Camera.Rotate(deltaX, -deltaY);
        }

        // ���� ���콺 ��ġ ������Ʈ
        lastMouseX = mouseX;
        lastMouseY = mouseY;
    }
    else {
        //  ��Ŭ���� �����Ǹ� firstClick �ʱ�ȭ
        firstClick = true;
    }
    Camera.SetProjection(1024.0f / 1024.0f, 0.1f, 1000.0f);
}


void UWorld::Render()
{
    renderer.Prepare();
    renderer.PrepareShader();

    // ���� ������Ʈ ������
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

        // ��ȯ ����: S * R * T
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

    // ���� ����� ������
    renderer.Graphics->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    FMatrix gizmoWorldMatrix = FMatrix::Identity;
    renderer.UpdateConstant(gizmoWorldMatrix* Camera.GetViewMatrix() * Camera.GetProjectionMatrix(),0);
    renderer.RenderPrimitive(vertexBufferGizmo, 6);

    imguiManager.BeginFrame();
    ImGui::Begin("Jungle Control Panel");
    ImGui::Text("Hello Jungle World");
    ImGui::Text("FPS 60");
    ImGui::Separator();
    // ���� ���õ� ������Ʈ Ÿ��
    static EObjectType selectedObjectType = EObjectType::Cube;

    // ��Ӵٿ�� ������ �̸� ����Ʈ
    const char* objectTypeNames[] = {"Cube", "Sphere" };

    ImGui::Text("Select Object Type");

    // ImGui ��Ӵٿ�� Enum ���� ������ �� �ֵ��� ��ȯ
    int selectedIndex = static_cast<int>(selectedObjectType);
    if (ImGui::Combo("##ObjectType", &selectedIndex, objectTypeNames, IM_ARRAYSIZE(objectTypeNames)))
    {
        selectedObjectType = static_cast<EObjectType>(selectedIndex);
    }

    // "Spawn" ��ư�� ������ �� ������Ʈ ����
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
    renderer.ReleaseVertexBuffer(vertexBufferGizmo); // ����� ���� ����
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
    // ���� �� ����� ���� (X, Y, Z)
    FVertexSimple gizmoVertices[] =
    {
        { 0.0f, 0.0f, 0.0f, 1, 0, 0, 1 }, { 100.0f, 0.0f, 0.0f, 1, 0, 0, 1 }, // X��
        { 0.0f, 0.0f, 0.0f, 0, 1, 0, 1 }, { 0.0f, 100.0f, 0.0f, 0, 1, 0, 1 }, // Y��
        { 0.0f, 0.0f, 0.0f, 0, 0, 1, 1 }, { 0.0f, 0.0f, 100.0f, 0, 0, 1, 1 }  // Z��
    };

    vertexBufferGizmo = renderer.CreateVertexBuffer(gizmoVertices, sizeof(gizmoVertices));

    //  Arrow�� ���� ����
    
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

    // NDC ��ǥ ���
    ndcX = (2.0f * mouseX) / 1024.0f - 1.0f;
    ndcY = 1.0f - (2.0f * mouseY) / 1024.0f;

    // NDC���� �� �������� ��ȯ
    FMatrix invProjection = FMatrix::Inverse(Camera.GetProjectionMatrix());
    FVector4 rayClip(ndcX, ndcY, 1.0f, 1.0f);
    FVector4 rayEye = FMatrix::TransformVector(rayClip, invProjection);
    rayEye.z = 1.0f;
    rayEye.a = 0.0f;

    // �� �������� ���� �������� ��ȯ
    FMatrix invView = FMatrix::Inverse(Camera.GetViewMatrix());
    FVector4 rayWorld4 = FMatrix::TransformVector(rayEye, invView);
    FVector rayWorld(rayWorld4.x, rayWorld4.y, rayWorld4.z);
    rayWorld = rayWorld.Normalize();

    // Ray�� ������ ���� ����
    lastRayOrigin = Camera.RelativeLocation;
    lastRayDirection = rayWorld;

    //  ����� ���� �˻�
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
    //  ���� ��ü ���� �˻�
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
        currentGizmo = nullptr;  // ������Ʈ�� ���������Ƿ� ���� ����� ���� ����
    }
}




//OBB�� ����
bool UWorld::RayIntersectsObject(const FVector& rayOrigin, const FVector& rayDir, USceneComponent* obj, float& hitDistance)
{
    // ������Ʈ�� ���� ��ȯ ��� ���� (��ġ, ȸ�� ����)
    FMatrix rotationMatrix = FMatrix::CreateRotation(
        obj->RelativeRotation.x,
        obj->RelativeRotation.y,
        obj->RelativeRotation.z
    );

    FMatrix translationMatrix = FMatrix::CreateTranslationMatrix(obj->RelativeLocation);

    // ���� ��ȯ ���
    FMatrix worldMatrix = rotationMatrix * translationMatrix;

    // OBB�� ���� X, Y, Z �� ��������
    FVector axisX(worldMatrix.M[0][0], worldMatrix.M[0][1], worldMatrix.M[0][2]);
    FVector axisY(worldMatrix.M[1][0], worldMatrix.M[1][1], worldMatrix.M[1][2]);
    FVector axisZ(worldMatrix.M[2][0], worldMatrix.M[2][1], worldMatrix.M[2][2]);

    // ��������� Ȯ��
    bool isGizmo = (dynamic_cast<UGizmoComp*>(obj) != nullptr);

    // OBB�� �� ũ�� ���� (������� ��� Y,Z �������� 0.2�� ����)
    FVector halfSize = obj->RelativeScale3D;

    FVector p = obj->RelativeLocation - rayOrigin; // ���� �������� OBB �߽ɱ����� ����
    FVector d = rayDir.Normalize(); // ���� ���� ����ȭ

    float tMin = -FLT_MAX, tMax = FLT_MAX;

    // �� ��(X, Y, Z)�� ���� ���̿��� �浹 �˻� ����
    FVector axes[3] = { axisX, axisY, axisZ };
    float halfSizes[3] = { halfSize.x, halfSize.y, halfSize.z };

    for (int i = 0; i < 3; i++)
    {
        float e = axes[i].Dot(p);  // �������� Ÿ�ٱ����� �Ÿ��� �ش� ������ ����
        float f = axes[i].Dot(d);  // ���̸� ����

        if (fabs(f) > 1e-6)  // ���� ���Ͱ� 0�̸� ū�ϳ�
        {
            float t1 = (e - halfSizes[i]) / f;
            float t2 = (e + halfSizes[i]) / f;

            if (t1 > t2) std::swap(t1, t2); // t1�� �׻� ���� ���� �ǵ��� ����

            tMin = max(tMin, t1);  // ������ ��� �ٿ�����
            tMax = min(tMax, t2);

            if (tMin > tMax) return false; // �浹 ����
        }
        else if (-e - halfSizes[i] > 0 || -e + halfSizes[i] < 0)
        {
            return false; // ���̰� OBB ���ο� ����, ���������� ����
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

    // X�� ����� (����)
    FVector xScale = FVector(max(scale.x * 1.5, 1), 0.2, 0.2);
    GizmoList[0]->SetTransform(location, rotation, xScale);

    // Y�� ����� (�ʷ�) - X�� Arrow�� Z�� �������� 90�� ȸ��
    FVector yScale = FVector(0.2, max(1, scale.y * 1.5), 0.2);
    FVector yRotation = FVector(rotation.x, rotation.y, rotation.z);
    GizmoList[1]->SetTransform(location, yRotation, yScale);

    // Z�� ����� (�Ķ�) - X�� Arrow�� Y�� �������� -90�� ȸ��
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

    // ������� ���� �̵� ���� ���� (X, Y, Z ��)
    FVector localMoveDirection(0, 0, 0);
    if (currentGizmo == GizmoList[0]) localMoveDirection = FVector(1, 0, 0);
    else if (currentGizmo == GizmoList[1]) localMoveDirection = FVector(0, 1, 0);
    else if (currentGizmo == GizmoList[2]) localMoveDirection = FVector(0, 0, 1);

    // ������� ���� ȸ�� ��� ��������
    FMatrix gizmoRotationMatrix = FMatrix::CreateRotation(
        currentGizmo->RelativeRotation.x,
        currentGizmo->RelativeRotation.y,
        currentGizmo->RelativeRotation.z
    );

    // ���� �̵� ���͸� ���� �������� ��ȯ
    FVector worldMoveDirection = FMatrix::TransformVector(localMoveDirection, gizmoRotationMatrix);
    worldMoveDirection = worldMoveDirection.Normalize();

    // ī�޶� �� ��� ��������
    FMatrix viewMatrix = Camera.GetViewMatrix();

    // ����� �̵� ������ �� �������� ��ȯ
    FVector viewMoveDirection = FMatrix::TransformVector(worldMoveDirection, viewMatrix);
    viewMoveDirection = viewMoveDirection.Normalize();

    // ���콺 �̵� ���� (��ũ�� ���� ����, ����ȭ)
    FVector mouseMoveDirection(deltaX, -deltaY, 0);
    mouseMoveDirection = mouseMoveDirection.Normalize();

    // ���콺 �̵� ���Ϳ� ����� �̵� ������ ������ �̿��� ���� ����
    float alignment = mouseMoveDirection.Dot(FVector(viewMoveDirection.x, viewMoveDirection.y, 0));
    float sign = (alignment < 0) ? -1.0f : 1.0f; // ���� ����

    // ������ �̵� ����
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






