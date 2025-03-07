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

	void SpawnObject(OBJECTS _Obj);
private:
	TArray<TDoubleLinkedList<UObject*>> m_pObjectList;
	UObject* PickingObj = nullptr;
public:
	UObject* GetCamera() { return m_pObjectList[OBJ_CAMERA].front(); }
	TDoubleLinkedList<UObject*>& GetSphreList() { return m_pObjectList[OBJ_SPHERE]; }
	TDoubleLinkedList<UObject*>& GetCubeList() { return m_pObjectList[OBJ_CUBE]; }
	UObject* GetPickingObj() { return PickingObj; }
};

