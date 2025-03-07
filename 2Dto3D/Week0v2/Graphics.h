#pragma once
#include <Windows.h>

class UGraphics
{
public:
    UGraphics();
    UGraphics(const UGraphics&);
    ~UGraphics();

    bool Initialize(int, int, HWND);
    void Release();
    bool Update();

private:
    bool Render();

private:

};