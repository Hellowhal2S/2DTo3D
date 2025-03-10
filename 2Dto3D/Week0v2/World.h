#pragma once
#include "Define.h"
#include "SceneMgr.h"
class UObject;

class UWorld
{
public:
	UWorld();
	~UWorld();

	void Initialize();
	void Tick(double deltaTime);
	void Release();
	void Render();
	void Input();
	void SpawnObject(OBJECTS _Obj);
	void LoadData(SceneData& _Data);
	SceneData SaveData();
	void NewScene();
private:
	//TArray<TDoubleLinkedList<UObject*>> m_pObjectList;
	TArray<UObject*> GUObjectArray;

	UObject* pickingObj = nullptr;
	UObject* pickingGizmo = nullptr;
	UObject* worldGizmo = nullptr;
	UObject* camera = nullptr;
	UObject* localPlayer = nullptr;
public:
	UObject* LocalGizmo[3] = { nullptr, nullptr, nullptr };
	UObject* GetCamera() { return camera; }
	UObject* GetPlayer() { return localPlayer; }


	TArray<UObject*> GetObjectArr() { return GUObjectArray; }
	UObject* GetPickingObj() { return pickingObj; }
	void	SetPickingObj(UObject* _Obj) { pickingObj = _Obj; };
	UObject* GetWorldGizmo() { return worldGizmo; }
	UObject* GetPickingGizmo() { return pickingGizmo; }
	void	SetPickingGizmo(UObject* _Obj) { pickingGizmo = _Obj; };

};

