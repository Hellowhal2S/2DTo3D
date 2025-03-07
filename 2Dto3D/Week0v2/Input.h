#pragma once

class UInput
{
public:
	UInput();
	UInput(const UInput&);
	~UInput();

public:
	bool Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool m_keys[256];
};