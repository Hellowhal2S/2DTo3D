#include "Input.h"

UInput::UInput()
{
}

UInput::UInput(const UInput& other)
{
}

UInput::~UInput()
{
}

bool UInput::Initialize()
{
	for (int i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}
	return true;
}

void UInput::KeyDown(unsigned int input)
{
	m_keys[input] = true;
	return;
}

void UInput::KeyUp(unsigned int input)
{
	m_keys[input] = false;
	return;
}

bool UInput::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}