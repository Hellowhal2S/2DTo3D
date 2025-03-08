#include "input.h"

InputClass::InputClass()
{
    directInput = 0;
    keyboard = 0;
    mouse = 0;
}

InputClass::~InputClass() {}

bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
    HRESULT result;

    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    mouseX = 0;
    mouseY = 0;

    result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL);
    if (FAILED(result)) return false;

    result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
    if (FAILED(result)) return false;

    result = keyboard->SetDataFormat(&c_dfDIKeyboard);
    if (FAILED(result)) return false;

    result = keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(result)) return false;

    result = keyboard->Acquire();
    if (FAILED(result)) return false;

    result = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
    if (FAILED(result)) return false;

    result = mouse->SetDataFormat(&c_dfDIMouse);
    if (FAILED(result)) return false;

    result = mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(result)) return false;

    result = mouse->Acquire();
    if (FAILED(result)) return false;

    return true;
}

void InputClass::Shutdown()
{
    if (mouse)
    {
        mouse->Unacquire();
        mouse->Release();
        mouse = 0;
    }

    if (keyboard)
    {
        keyboard->Unacquire();
        keyboard->Release();
        keyboard = 0;
    }

    if (directInput)
    {
        directInput->Release();
        directInput = 0;
    }
}

bool InputClass::Frame()
{
    if (!ReadKeyboard() || !ReadMouse())
        return false;

    ProcessInput();
    return true;
}

bool InputClass::ReadKeyboard()
{
    HRESULT result = keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
    if (FAILED(result))
    {
        if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
        {
            keyboard->Acquire();
        }
        else
        {
            return false;
        }
    }
    return true;
}

bool InputClass::ReadMouse()
{
    HRESULT result = mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
    if (FAILED(result))
    {
        if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
        {
            mouse->Acquire();
        }
        else
        {
            return false;
        }
    }
    return true;
}

void InputClass::ProcessInput()
{
    mouseX += mouseState.lX;
    mouseY += mouseState.lY;

    if (mouseX < 0) mouseX = 0;
    if (mouseY < 0) mouseY = 0;
    if (mouseX > screenWidth) mouseX = screenWidth;
    if (mouseY > screenHeight) mouseY = screenHeight;
}

bool InputClass::IsEscapePressed()
{
    return (keyboardState[DIK_ESCAPE] & 0x80) != 0;
}

bool InputClass::IsLeftArrowPressed()
{
    return (keyboardState[DIK_LEFT] & 0x80) != 0;
}

bool InputClass::IsRightArrowPressed()
{
    return (keyboardState[DIK_RIGHT] & 0x80) != 0;
}

bool InputClass::IsKeyPressed(unsigned char key)
{
    return (keyboardState[key] & 0x80) != 0;
}

void InputClass::GetMouseLocation(int& x, int& y)
{
    x = mouseX;
    y = mouseY;
}

bool InputClass::IsMousePressed()
{
    return (mouseState.rgbButtons[0] & 0x80) != 0;
}

bool InputClass::IsRightMousePressed()
{
    return (mouseState.rgbButtons[1] & 0x80) != 0;
}

