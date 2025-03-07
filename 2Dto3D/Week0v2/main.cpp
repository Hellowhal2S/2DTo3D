#include <Windows.h>
#include "World.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    UWorld world;
    world.Initialize(hInstance);
    world.Run();
    world.Release();

    return 0;
}
