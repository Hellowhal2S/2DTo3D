#include "CameraComponent.h"
#include <windows.h>

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

}

void UCameraComponent::MoveForward(float _Value)
{
	m_Location.x += _Value;
}

void UCameraComponent::MoveRight(float _Value)
{
	m_Location.y += _Value;
}
