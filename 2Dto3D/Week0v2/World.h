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
	void Input();
	void SpawnObject(OBJECTS _Obj);
private:
	TArray<TDoubleLinkedList<UObject*>> m_pObjectList;
	UObject* pickingObj = nullptr;
	UObject* pickingGizmo = nullptr;
	UObject* worldGizmo = nullptr;
public:
	UObject* LocalGizmo[3] = { nullptr, nullptr, nullptr };
	UObject* GetCamera() { return m_pObjectList[OBJ_CAMERA].front(); }
	TDoubleLinkedList<UObject*>& GetSphreList() { return m_pObjectList[OBJ_SPHERE]; }
	TDoubleLinkedList<UObject*>& GetCubeList() { return m_pObjectList[OBJ_CUBE]; }
	UObject* GetPickingObj() { return pickingObj; }
	void	SetPickingObj(UObject* _Obj) { pickingObj = _Obj; };
	UObject* GetWorldGizmo() { return worldGizmo; }
	UObject* GetPickingGizmo() { return pickingGizmo; }
	void	SetPickingGizmo(UObject* _Obj) { pickingGizmo = _Obj; };

};

