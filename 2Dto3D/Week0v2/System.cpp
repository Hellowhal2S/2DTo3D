#include "System.h"

USystem::USystem()
{
	m_Input = NULL;
	m_Graphics = NULL;
}

USystem::USystem(const USystem& other)
{
}

USystem::~USystem()
{
}

bool USystem::Initialize()
{
	int screenWidth, screenHeight;
	bool bResult;

	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create the input object.
	m_Input = new UInput;
	if (!m_Input)
	{
		return false;
	}
	// Initialize the input object.
	m_Input->Initialize();

	// Create the graphics object.
	m_Graphics = new UGraphics;
	if (!m_Graphics)
	{
		return false;
	}
	// Initialize the graphics object.
	bResult = m_Graphics->Initialize(screenWidth, screenHeight, m_hWnd);
	if (!bResult)
	{
		return false;
	}

	return true;
}

void USystem::Release()
{
	// Release the graphics object.
	if (m_Graphics)
	{
		m_Graphics->Release();
		delete m_Graphics;
		m_Graphics = NULL;
	}
	// Release the input object.
	if (m_Input)
	{
		delete m_Input;
		m_Input = NULL;
	}
	// Shutdown the window.
	ShutdownWindows();
	return;
}

void USystem::Run()
{
	bool bResult;
	MSG msg;
	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	// Loop until there is a quit message from the window or the user.
	bool bIsExit = false;
	while (bIsExit == false)
	{
		// Handle the windows messages.
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg); // 키보드 입력 메시지를 문자메시지로 변경
			DispatchMessage(&msg); // 메시지를 WndProc에 전달
			// If windows signals to end the application then exit out.
			if (msg.message == WM_QUIT)
			{
				bIsExit = true;
				break;
			}
			else
			{
				// Otherwise do the frame processing.
				bResult = Update();
				if (!bResult)
				{
					bIsExit = true;
				}
			}
		}
	}
	return;
}

LRESULT CALLBACK USystem::MessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	// Check Keyboard input
	case WM_KEYDOWN:
		{
			m_Input->KeyDown((uint32)wParam);
			return 0;
		}
	case WM_KEYUP:
		{
			m_Input->KeyUp((uint32)wParam);
			return 0;
		}

	default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
}

bool USystem::Update()
{
	bool bResult;

	// Check if the user pressed escape and wants to exit the application.
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	bResult = m_Graphics->Update();
	if (!bResult)
	{
		return false;
	}

	return true;
}

void USystem::InitializeWindows(int& screenWidth, int& screenHeight)
{
	ApplicationHandle = this;

	m_applicationName = L"JungleWindowClass";

	WCHAR Title[] = L"Game Tech Lab";

	WNDCLASSW wndclass = { 0, WndProc, 0,0,0,0,0,0,0, m_applicationName };

	RegisterClassW(&wndclass);

	m_hWnd = CreateWindowExW(0, m_applicationName, Title, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1024, 1024,
		nullptr, nullptr, m_hInstance, nullptr);

	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);
}

void USystem::ShutdownWindows()
{
	DestroyWindow(m_hWnd);
	m_hWnd = NULL;

	UnregisterClassW(m_applicationName, m_hInstance);

	ApplicationHandle = NULL;
	return;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
	{
		return true;
	}
	switch (message)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		// Check if the window is being closed.
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		// All other messages pass to the message handler in the system class.
	default:
		{
			return ApplicationHandle->MessageHandler(hWnd, message, wParam, lParam);
		}
	}
}