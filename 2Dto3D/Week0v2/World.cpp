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

    if (InputHandler.IsMousePressed())
    {
        SelectObjectWithMouse();
    }


    //  ���콺 �Է� ó�� (��Ŭ�� �� ȸ�� ����)
    static int lastMouseX = 0, lastMouseY = 0;
    int mouseX, mouseY;

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
        FVector location = currentObject->RelativeLocation;
        FVector scale = currentObject->RelativeScale3D;

        // ��ü�� �������� ��ȯ
        
        FMatrix objectTransform = FMatrix::CreateTranslationMatrix(location);
        FMatrix objectRotation = FMatrix::CreateRotation(currentObject->RelativeRotation.x,
            currentObject->RelativeRotation.y,
            currentObject->RelativeRotation.z);

        // X�� ����� (����)
        FMatrix scaleMatrix = FMatrix::CreateScale(max(scale.x * 2, 1), 1, 1);
        FMatrix gizmoX = scaleMatrix * objectRotation * objectTransform;
        FMatrix matrix =  Camera.GetViewMatrix() * Camera.GetProjectionMatrix();
        renderer.UpdateConstant(gizmoX * matrix,1);
        renderer.RenderPrimitive(vertexBufferArrow, sizeof(arrow_vertices) / sizeof(FVertexSimple));

        // Y�� ����� (�ʷ�) - X�� Arrow�� Z�� �������� 90�� ȸ��
         scaleMatrix = FMatrix::CreateScale(1, max(1,scale.y * 2), 1);
        FMatrix rotationY = FMatrix::CreateRotation(0, 0, 90);
        FMatrix gizmoY = rotationY * scaleMatrix * objectRotation * objectTransform;
        renderer.UpdateConstant(gizmoY* matrix,2);
        renderer.RenderPrimitive(vertexBufferArrow, sizeof(arrow_vertices) / sizeof(FVertexSimple));

        // Z�� ����� (�Ķ�) - X�� Arrow�� Z�� �������� 90�� ȸ��
        scaleMatrix = FMatrix::CreateScale(1, 1 , max(1, scale.z * 2));
        FMatrix rotationZ = FMatrix::CreateRotation(0, -90, 0);
        FMatrix gizmoZ = rotationZ * scaleMatrix * objectRotation * objectTransform;
        renderer.UpdateConstant(gizmoZ*matrix,3);
        renderer.RenderPrimitive(vertexBufferArrow, sizeof(arrow_vertices) / sizeof(FVertexSimple));
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
    vertexBufferArrow = renderer.CreateVertexBuffer(arrow_vertices, sizeof(arrow_vertices));
}




void UWorld::SelectObjectWithMouse()
{
    int mouseX, mouseY;
    InputHandler.GetMouseAbsolutePosition(mouseX, mouseY); // ���� InputHandler�� ���콺 �̵� �� ���� -> window input���� ����

    // NDC ��ǥ ���
    ndcX = (2.0f * mouseX) / 1024.0f - 1.0f; // X��: [-1, 1]
    ndcY = 1.0f - (2.0f * mouseY) / 1024.0f; // Y��: [1, -1]

    // NDC���� �� �������� ��ȯ
    FMatrix invProjection = FMatrix::Inverse(Camera.GetProjectionMatrix());
    FVector4 rayClip(ndcX, ndcY, 1.0f, 1.0f); // Z = 1.0f (�� ���������� far plane)
    FVector4 rayEye = FMatrix::TransformVector(rayClip, invProjection);
    rayEye.z = 1.0f; // �� ���������� ���� ���� (Z = 1.0f)
    rayEye.a = 0.0f; // ���� �����̹Ƿ� w = 0

    // �� �������� ���� �������� ��ȯ
    FMatrix invView = FMatrix::Inverse(Camera.GetViewMatrix());
    FVector4 rayWorld4 = FMatrix::TransformVector(rayEye, invView);
    FVector rayWorld(rayWorld4.x, rayWorld4.y, rayWorld4.z);
    rayWorld = rayWorld.Normalize();

    // Ray�� ������ ���� ����
    lastRayOrigin = Camera.RelativeLocation;
    lastRayDirection = rayWorld;



    // �浹 �˻�
    float closestDistance = FLT_MAX;
    USceneComponent* selectedObject = nullptr;

    for (auto& Object : ObjectList)
    {
        USceneComponent* sceneComp = dynamic_cast<USceneComponent*>(Object);
        if (sceneComp)
        {
            float hitDistance;
            if (RayIntersectsObject(lastRayOrigin, lastRayDirection, sceneComp, hitDistance))
            {
                if (hitDistance < closestDistance)
                {
                    closestDistance = hitDistance;
                    selectedObject = sceneComp;
                }
            }
        }
    }

    // ���� ����� ������Ʈ�� ����
    if (selectedObject)
    {
        currentObject = selectedObject;
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

    // ���� ��ȯ ��� (Scale�� ���߿� ����)
    FMatrix worldMatrix = rotationMatrix * translationMatrix;

    //OBB�� ���� X, Y, Z ����
    FVector axisX(worldMatrix.M[0][0], worldMatrix.M[0][1], worldMatrix.M[0][2]);
    FVector axisY(worldMatrix.M[1][0], worldMatrix.M[1][1], worldMatrix.M[1][2]);
    FVector axisZ(worldMatrix.M[2][0], worldMatrix.M[2][1], worldMatrix.M[2][2]);

    // OBB�� �� ũ�� ���� (�� �ະ ũ�� �ݿ�)
    FVector halfSize = obj->RelativeScale3D;

    FVector p = obj->RelativeLocation - rayOrigin; // ���� �������� OBB �߽ɱ����� ����
    FVector d = rayDir.Normalize(); // ���� ���� ����ȭ

    float tMin = -FLT_MAX, tMax = FLT_MAX;

    // �� ��(X, Y, Z)�� ���� ���̿��� �浹 �˻� ����
    FVector axes[3] = { axisX, axisY, axisZ };
    float halfSizes[3] = { halfSize.x, halfSize.y,halfSize.z}; // scale ���

    for (int i = 0; i < 3; i++)
    {
        float e = axes[i].Dot(p);  // �������� Ÿ�ٱ����� �Ÿ��� �ش� ������ ����
        float f = axes[i].Dot(d);  // ray�� ����

        if (fabs(f) > 1e-6)  // ���� ���Ͱ� 0�̸� ������ ����
        {
            float t1 = (e - halfSizes[i]) / f;
            float t2 = (e + halfSizes[i]) / f;

            if (t1 > t2) std::swap(t1, t2); // t1�� �׻� ���� ���� �ǵ��� ����

            tMin = max(tMin, t1);
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










