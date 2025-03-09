#pragma once
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <windows.h>
#include <dinput.h>

class InputClass
{
public:
    InputClass();
    ~InputClass();

    bool Initialize(HINSTANCE, HWND, int, int);
    void Shutdown();
    bool Frame();

    bool IsEscapePressed();
    bool IsLeftArrowPressed();
    bool IsRightArrowPressed();
    bool IsKeyPressed(unsigned char key);

    void GetMouseLocation(int&, int&);
    bool IsMousePressed();
    bool IsRightMousePressed();
    void GetMouseAbsolutePosition(int& x, int& y);

private:
    bool ReadKeyboard();
    bool ReadMouse();
    void ProcessInput();

private:
    IDirectInput8* directInput;
    IDirectInputDevice8* keyboard;
    IDirectInputDevice8* mouse;
    unsigned char keyboardState[256];
    DIMOUSESTATE mouseState;
    int screenWidth, screenHeight, mouseX, mouseY;
};

#endif
