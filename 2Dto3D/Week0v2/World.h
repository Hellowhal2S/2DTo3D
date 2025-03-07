#pragma once
#include "Define.h"

class UObject;
class UWorld
{
public:
	UWorld();
	~UWorld();

	void Initialize();
	void Update(double deltaTime);
	void Release();
	void Render();

private:
	TArray<TDoubleLinkedList<UObject*>> m_pObjectList;
public:
	UObject* GetCamera() { return m_pObjectList[OBJ_CAMERA].front(); }
};

