#include "World.h"
#include "sphere.h"

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

void UWorld::Initialize(HINSTANCE hInstance) {
    UCubeComp* cube1 = new UCubeComp();
    UCubeComp* cube2 = new UCubeComp();
    USphereComp* sphere = new USphereComp();

    cube1->Initialize(&graphicDevice);
    cube2->Initialize(&graphicDevice);
    sphere->Initialize(&graphicDevice);

    AddObject(cube1);
    AddObject(cube2);
    AddObject(sphere);
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
}

void UWorld::Render()
{
    renderer.Prepare();
    renderer.PrepareShader();

    FMatrix viewMatrix = Camera.GetViewMatrix();
    FMatrix projectionMatrix = Camera.GetProjectionMatrix();
    FMatrix worldMatrix = FMatrix::Identity; // �⺻ ���� ���

    renderer.UpdateConstant(worldMatrix, viewMatrix, projectionMatrix);
    renderer.RenderPrimitive(vertexBufferSphere, sizeof(sphere_vertices) / sizeof(FVertexSimple));

    imguiManager.BeginFrame();
    ImGui::Begin("Jungle Control Panel");
    ImGui::Text("Hello Jungle World");
    ImGui::Text("FPS 60");
    ImGui::Separator();
    ImGui::SliderFloat("##FOV", &Camera.fovDegrees, 0.0f, 180.0f, "%.1f");
    float position[3] = { Camera.RelativeLocation.x, Camera.RelativeLocation.y, Camera.RelativeLocation.z };
    ImGui::PushItemWidth(150); // �׸� �ڽ� ũ�� ����
    ImGui::InputFloat3("Position", position, "%.2f", ImGuiInputTextFlags_ReadOnly);
    // LookAt ���͸� �׸� �ڽ��� ǥ��
    float lookAt[3] = { Camera.GetForward().x, Camera.GetForward().y, Camera.GetForward().z };
    ImGui::InputFloat3("LookAt", lookAt, "%.2f", ImGuiInputTextFlags_ReadOnly);
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



