#pragma once
#include "Define.h"

class UObject;
class UWorld
{
public:
	UWorld();
	~UWorld();

	void Initialize();
	void Update();
	void Release();
	void Render();

private:
	TArray<TDoubleLinkedList<UObject*>> m_pObjectList;
};

