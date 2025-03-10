#include "EngineLoop.h"

#include "JungleMath.h"

#include "ImGuiManager.h"
#include "ControlPanel.h"
#include "PropertyPanel.h"

#include "World.h"

#include "CameraComponent.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
	{
		return true;
	}

	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED) {
			//UGraphicsDevice ��ü�� OnResize �Լ� ȣ��
			if (FEngineLoop::graphicDevice.SwapChain) {
				FEngineLoop::graphicDevice.OnResize(hWnd, GEngineLoop.aspectRatio);
			}
		}
	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	}

	return 0;
}

FGraphicsDevice FEngineLoop::graphicDevice;
FRenderer FEngineLoop::renderer;
FResourceManager FEngineLoop::resourceManager;

FEngineLoop::FEngineLoop()
{
}

FEngineLoop::~FEngineLoop()
{
}

int32 FEngineLoop::PreInit()
{
	return 0;
}

int32 FEngineLoop::Init(HINSTANCE hInstance)
{
	int screenWidth, screenHeight;
	bool fullScreen;

	LoadEngineConfig(screenWidth, screenHeight, fullScreen);
	WindowInit(hInstance, screenWidth, screenHeight);

	graphicDevice.Initialize(hWnd);

	renderer.Initialize(&graphicDevice);

	UIManager = new UImGuiManager;
	UIManager->Initialize(hWnd, graphicDevice.Device, graphicDevice.DeviceContext);

	resourceManager.Initialize(&renderer);

	GWorld = new UWorld;
	GWorld->Initialize();

	return 0;
}

void FEngineLoop::Tick()
{
	LARGE_INTEGER frequency;
	const double targetFrameTime = 1000.0 / targetFPS; // �� �������� ��ǥ �ð� (�и��� ����)

	QueryPerformanceFrequency(&frequency);

	LARGE_INTEGER startTime, endTime;
	double elapsedTime = 1.0;

	while (bIsExit == false)
	{
		QueryPerformanceCounter(&startTime);

		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg); // Ű���� �Է� �޽����� ���ڸ޽����� ����
			DispatchMessage(&msg); // �޽����� WndProc�� ����

			if (msg.message == WM_QUIT)
			{
				bIsExit = true;
				break;
			}
		}

		// ���� ���� ������Ʈ
		GWorld->Tick(elapsedTime);

		// ī�޶� ���� VP Matrix ����
		UCameraComponent* Camera = static_cast<UCameraComponent*>(GWorld->GetCamera());
		View = JungleMath::CreateViewMatrix(Camera->GetLocation(), Camera->GetLocation() + Camera->GetForwardVector(), { 0, 1, 0 });
		Projection = JungleMath::CreateProjectionMatrix(Camera->GetFov() * (PI / 180.0f), aspectRatio, 0.1f, 1000.0f);

		graphicDevice.Prepare();
		renderer.PrepareShader();

		Render();

		UIManager->BeginFrame();

		Console::GetInstance().Draw();
		ControlPanel::GetInstance().Draw(GetWorld(), elapsedTime);
		PropertyPanel::GetInstance().Draw(GetWorld());

		UIManager->EndFrame();

		// ȭ�� ����
		graphicDevice.SwapBuffer();

		do
		{
			Sleep(0);
			QueryPerformanceCounter(&endTime);
			elapsedTime = (endTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart;
		} while (elapsedTime < targetFrameTime);
	}
}

void FEngineLoop::Render()
{
	for (auto iter : GetWorld()->GetObjectArr())
	{
		iter->Render();
	}
}

void FEngineLoop::Exit()
{
	GWorld->Release();
	delete GWorld;

	resourceManager.Release(&renderer);

	renderer.Release();
	graphicDevice.Release();

	UIManager->Shutdown();
	delete UIManager;
}

void FEngineLoop::WindowInit(HINSTANCE hInstance, int screenWidth, int screenHeight)
{
	WCHAR WindowClass[] = L"JungleWindowClass";

	WCHAR Title[] = L"Game Tech Lab";

	WNDCLASSW wndclass = { 0 };
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.lpszClassName = WindowClass;

	RegisterClassW(&wndclass);

	hWnd = CreateWindowExW(0, WindowClass, Title, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, screenWidth, screenHeight,
		nullptr, nullptr, hInstance, nullptr);
}

void FEngineLoop::LoadEngineConfig(int& screenWidth, int& screenHeight, bool& fullScreen)
{
	std::ifstream configFile("engine.ini");
	if (configFile.is_open())
	{
		FString line;
		while (std::getline(configFile, line))
		{
			// ������ �Ľ� ����
			if (line.find("ScreenWidth=") != FString::npos)
			{
				screenWidth = std::stof(line.substr(line.find("=") + 1));
			}
			else if (line.find("ScreenHeight=") != FString::npos)
			{
				screenHeight = std::stof(line.substr(line.find("=") + 1));
			}
			else if (line.find("FullScreen=") != FString::npos)
			{
				FString value = line.substr(line.find("=") + 1);
				fullScreen = (value == "true" || value == "1" || value == "True");
			}
		}
		configFile.close();
	}
	else
	{
		// ������ ���� ���
		// WinAPI���� �⺻ �ػ󵵸� ������
		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);
		fullScreen = false;

		std::ofstream newConfigFile("engine.ini");
		newConfigFile << "ScreenWidth=" << screenWidth << "\n" << std::endl;
		newConfigFile << "ScreenHeight=" << screenHeight << "\n" << std::endl;
		newConfigFile << "FullScreen=" << (fullScreen ? "true" : "false") << "\n" << std::endl;
		newConfigFile.close();
	}

	aspectRatio = (float)screenWidth / (float)screenHeight;
}
