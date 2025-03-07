#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "UEContainerAliases.h"

#include "Input.h"
#include "Graphics.h"

class USystem
{
public:
    USystem();
    USystem(const USystem&);
    ~USystem();

    bool Initialize();
    void Release();
    void Run();

    LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
    bool Update();
    void InitializeWindows(int&, int&);
    void ShutdownWindows();

private:
    LPCWSTR m_applicationName;
    HINSTANCE m_hInstance;
    HWND m_hWnd;

    UInput* m_Input;
    UGraphics* m_Graphics;
};

// FUNCTION PROTOTYPES //
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// GLOBALS //
static USystem* ApplicationHandle = NULL;
