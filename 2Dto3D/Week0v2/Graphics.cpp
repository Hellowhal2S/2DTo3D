#include "Graphics.h"


UGraphics::UGraphics()
{
}


UGraphics::UGraphics(const UGraphics& other)
{
}


UGraphics::~UGraphics()
{
}


bool UGraphics::Initialize(int screenWidth, int screenHeight, HWND hWnd)
{
    bool bResult;

    // Direct3D 객체를 생성합니다.
    m_device = new UDevice;
    if (!m_device)
    {
        return false;
    }

    // Direct3D 객체를 초기화합니다.
    bResult = m_device->Initialize(screenWidth, screenHeight, hWnd);
    if (!bResult)
    {
        MessageBox(hWnd, L"Could not initialize Direct3D", L"Error", MB_OK);
        return false;
    }

    return true;
}


void UGraphics::Release()
{
    // D3D 객체를 반환합니다.
    if (m_device)
    {
        m_device->Release();
        delete m_device;
        m_device = NULL;
    }

    return;
}


bool UGraphics::Update()
{
    bool bResult;

    // 그래픽 렌더링을 수행합니다.
    bResult = Render();
    if (!bResult)
    {
        return false;
    }

    return true;
}


bool UGraphics::Render()
{
    // 씬 그리기를 시작하기 위해 버퍼의 내용을 지웁니다.
    m_device->BeginScene();

    // 버퍼에 그려진 씬을 화면에 표시합니다.
    m_device->EndScene();

    return true;
}