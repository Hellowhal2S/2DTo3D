#include "CameraComponent.h"

UCameraComponent::UCameraComponent()
{
}

UCameraComponent::~UCameraComponent()
{
}

void UCameraComponent::Initialize(UWorld* _World)
{
	Super::Initialize(_World);
	m_Location = FVector(0.0f, 0.0f, 0.5f);
}

void UCameraComponent::Update(double deltaTime)
{
	Input();
}

void UCameraComponent::Release()
{
}

void UCameraComponent::Input()
{
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) // VK_RBUTTON�� ���콺 ������ ��ư�� ��Ÿ��
	{
		if (!bRightMouseDown)
		{
			// ���콺 ������ ��ư�� ó�� ������ ��, ���콺 ��ġ �ʱ�ȭ
			bRightMouseDown = true;
			GetCursorPos(&m_LastMousePos);
		}
		else
		{
			// ���콺 �̵��� ���
			POINT currentMousePos;
			GetCursorPos(&currentMousePos);

			// ���콺 �̵� ���� ���
			int deltaX = currentMousePos.x - m_LastMousePos.x;
			int deltaY = currentMousePos.y - m_LastMousePos.y;

			// Yaw(�¿� ȸ��) �� Pitch(���� ȸ��) �� ����
			RotateYaw(deltaX * 0.1f);  // X �̵��� ���� �¿� ȸ��
			RotatePitch(deltaY * 0.1f);  // Y �̵��� ���� ���� ȸ��

			// ���ο� ���콺 ��ġ ����
			m_LastMousePos = currentMousePos;
		}
	}
	else
	{
		bRightMouseDown = false; // ���콺 ������ ��ư�� ���� ���� �ʱ�ȭ
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		MoveRight(-1.f);
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		MoveRight(1.f);
	}
	if (GetAsyncKeyState('W') & 0x8000)
	{
		MoveForward(1.f);
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		MoveForward(-1.f);
	}
	if (GetAsyncKeyState('E') & 0x8000)
	{
		MoveUp(1.f);
	}
	if (GetAsyncKeyState('Q') & 0x8000)
	{
		MoveUp(-1.f);
	}
	if (GetAsyncKeyState('J') & 0x8000)
	{
		RotateYaw(-1.f);
	}
	if (GetAsyncKeyState('L') & 0x8000)
	{
		RotateYaw(1.f);
	}
}

void UCameraComponent::MoveForward(float _Value)
{
	m_Location = m_Location + GetForwardVector() * m_mouseSpeed * _Value;
}

void UCameraComponent::MoveRight(float _Value)
{
	m_Location = m_Location + GetRightVector() * m_mouseSpeed * _Value;
}

void UCameraComponent::MoveUp(float _Value)
{
	m_Location.y += _Value *0.5f;
}

void UCameraComponent::RotateYaw(float _Value)
{
	m_Rotation.y -= _Value;
}

void UCameraComponent::RotatePitch(float _Value)
{
	m_Rotation.x -= _Value;
}

