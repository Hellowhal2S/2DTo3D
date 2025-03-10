#pragma once

#include "ImGuiManager.h"

#include "Object.h"

class UWorld;

class PropertyPanel
{
private:
	PropertyPanel();

public:
	~PropertyPanel();
	static PropertyPanel GetInstance()
	{
		static PropertyPanel Inst;
		return Inst;
	}
	void Draw(UWorld* world);
};
