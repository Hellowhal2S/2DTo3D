#include "World.h"
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
    Camera.SetProjection(45.0f, 1024.0f / 1024.0f, 0.1f, 1000.0f);
    graphicDevice.Initialize(hWnd);
    renderer.Initialize(&graphicDevice);
    imguiManager.Initialize(hWnd, graphicDevice.Device, graphicDevice.DeviceContext);

    UINT numVerticesSphere = sizeof(sphere_vertices) / sizeof(FVertexSimple);
    vertexBufferSphere = renderer.CreateVertexBuffer(sphere_vertices, sizeof(sphere_vertices));
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
}





void UWorld::Render()
{
    renderer.Prepare();
    renderer.PrepareShader();

    FMatrix viewMatrix = Camera.GetViewMatrix();
    FMatrix projectionMatrix = Camera.GetProjectionMatrix();
    FMatrix worldMatrix = FMatrix::Identity; // 기본 월드 행렬

  
    renderer.UpdateConstant(worldMatrix, viewMatrix, projectionMatrix);

    renderer.RenderPrimitive(vertexBufferSphere, sizeof(sphere_vertices) / sizeof(FVertexSimple));

    imguiManager.BeginFrame();
    ImGui::Begin("Camera Debug");
    ImGui::Text("Position: (%.2f, %.2f, %.2f)", Camera.RelativeLocation.x, Camera.RelativeLocation.y, Camera.RelativeLocation.z);
    ImGui::Text("LookAt: (%.2f, %.2f, %.2f)", Camera.GetForward().x, Camera.GetForward().y, Camera.GetForward().z);
    ImGui::End();
    imguiManager.Render();
}


void UWorld::Release()
{
    renderer.ReleaseVertexBuffer(vertexBufferSphere);
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



