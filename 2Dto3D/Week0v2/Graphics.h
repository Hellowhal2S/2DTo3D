#pragma once
#include <Windows.h>
#include "Device.h"

class UGraphics
{
public:
    UGraphics();
    UGraphics(const UGraphics&);
    ~UGraphics();

public:
    bool Initialize(int, int, HWND);
    void Release();
    bool Update();

private:
    bool Render();

private:
	UDevice* m_device;
};