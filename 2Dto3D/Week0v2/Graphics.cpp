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

    // Direct3D ��ü�� �����մϴ�.
    m_device = new UDevice;
    if (!m_device)
    {
        return false;
    }

    // Direct3D ��ü�� �ʱ�ȭ�մϴ�.
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
    // D3D ��ü�� ��ȯ�մϴ�.
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

    // �׷��� �������� �����մϴ�.
    bResult = Render();
    if (!bResult)
    {
        return false;
    }

    return true;
}


bool UGraphics::Render()
{
    // �� �׸��⸦ �����ϱ� ���� ������ ������ ����ϴ�.
    m_device->BeginScene();

    // ���ۿ� �׷��� ���� ȭ�鿡 ǥ���մϴ�.
    m_device->EndScene();

    return true;
}