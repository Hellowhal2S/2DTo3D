#include "World.h"
#include "Cube.h"
#include "Sphere.h"

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

    //  마우스 입력 처리 (우클릭 시 회전 가능)
    static int lastMouseX = 0, lastMouseY = 0;
    int mouseX, mouseY;
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

    // 기존 큐브와 스피어 렌더링
    for (auto& Object : ObjectList)
    {
        USceneComponent* sceneComp = dynamic_cast<USceneComponent*>(Object);
        FMatrix translationMatrix = FMatrix::Identity;
        translationMatrix.M[3][0] = sceneComp->RelativeLocation.x;
        translationMatrix.M[3][1] = sceneComp->RelativeLocation.y;
        translationMatrix.M[3][2] = sceneComp->RelativeLocation.z;

        // 회전 행렬 적용 (Yaw, Pitch, Roll)
        FMatrix rotationMatrix = FMatrix::CreateRotation(sceneComp->RelativeRotation.x,  // Roll
            sceneComp->RelativeRotation.y,  // Pitch
            sceneComp->RelativeRotation.z); // Yaw

        // 스케일 행렬 적용
        FMatrix scaleMatrix = FMatrix::CreateScale(sceneComp->RelativeScale3D.x,
            sceneComp->RelativeScale3D.y,
            sceneComp->RelativeScale3D.z);

        // 최종 변환 행렬 (S * R * T)
        FMatrix worldMatrix = scaleMatrix * rotationMatrix * translationMatrix;
        if (UCubeComp* Cube = dynamic_cast<UCubeComp*>(Object))
        {
            FMatrix worldMatrix = scaleMatrix * rotationMatrix * translationMatrix;

            renderer.UpdateConstant(worldMatrix, Camera.GetViewMatrix(), Camera.GetProjectionMatrix());
            renderer.RenderPrimitive(vertexBufferCube, sizeof(cube_vertices) / sizeof(FVertexSimple));
        }

        if (USphereComp* Sphere = dynamic_cast<USphereComp*>(Object))
        {
            renderer.UpdateConstant(worldMatrix, Camera.GetViewMatrix(), Camera.GetProjectionMatrix());
            renderer.RenderPrimitive(vertexBufferSphere, sizeof(sphere_vertices) / sizeof(FVertexSimple));
        }
    }

    // 기즈모 렌더링
    renderer.Graphics->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    FMatrix gizmoWorldMatrix = FMatrix::Identity;
    renderer.UpdateConstant(gizmoWorldMatrix, Camera.GetViewMatrix(), Camera.GetProjectionMatrix());
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

    ImGui::Checkbox("Orthgraphic", &Camera.bOrthographic);
    ImGui::SliderFloat("##FOV", &Camera.fovDegrees, 0.0f, 180.0f, "%.1f");
    float position[3] = { Camera.RelativeLocation.x, Camera.RelativeLocation.y, Camera.RelativeLocation.z };
    ImGui::PushItemWidth(150);
    ImGui::InputFloat3("Position", position, "%.2f", ImGuiInputTextFlags_ReadOnly);
    float lookAt[3] = { Camera.GetForward().x, Camera.GetForward().y, Camera.GetForward().z };
    ImGui::InputFloat3("LookAt", lookAt, "%.2f", ImGuiInputTextFlags_ReadOnly);
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
	currentObject = NewCube;

}  

void UWorld::SpawnSphere()
{
	USphereComp* NewSphere = new USphereComp();
	AddObject(NewSphere);
}

void UWorld::CreateGizmo()
{
    // X (Red), Y (Green), Z (Blue) 축을 표현하는 선분 버텍스
    FVertexSimple gizmoVertices[] =
    {
        // X축 (빨강)
   { 0.0f, 0.0f, 0.0f, 1, 0, 0, 1 }, // 원점
   { 10.0f, 0.0f, 0.0f, 1, 0, 0, 1 }, // 끝점

   // Y축 (초록)
   { 0.0f, 0.0f, 0.0f, 0, 1, 0, 1 },
   { 0.0f, 10.0f, 0.0f, 0, 1, 0, 1 },

   // Z축 (파랑)
   { 0.0f, 0.0f, 0.0f, 0, 0, 1, 1 },
   { 0.0f, 0.0f, 10.0f, 0, 0, 1, 1 },
    };

    // 기즈모 버퍼 생성
    vertexBufferGizmo = renderer.CreateVertexBuffer(gizmoVertices, sizeof(gizmoVertices));
}



