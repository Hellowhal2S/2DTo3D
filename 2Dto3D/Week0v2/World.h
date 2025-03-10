#pragma once

#include "Define.h"

#include "SceneManager.h"

class UObject;

class UWorld
{
public:
	UWorld();
	~UWorld();

	void Initialize();
	void Release();
	void Tick(double deltaTime);
	void Render();

	void Input();
	
	void SpawnObject(OBJECTS _Obj);

	void LoadData(SceneData& _Data);
	SceneData SaveData();
	void NewScene();

private:
	UObject* camera = nullptr;

	TArray<UObject*> GUObjectArray;

	UObject* pickingObj = nullptr;
	UObject* pickingGizmo = nullptr;
	UObject* worldGizmo = nullptr;

public:
	UObject* LocalGizmo[3] = { nullptr, nullptr, nullptr };
	UObject* GetCamera() { return camera; }

	TArray<UObject*> GetObjectArr() { return GUObjectArray; }

	UObject* GetPickingObj() { return pickingObj; }
	void	SetPickingObj(UObject* _Obj) { pickingObj = _Obj; };

	UObject* GetWorldGizmo() { return worldGizmo; }
	UObject* GetPickingGizmo() { return pickingGizmo; }
	void	SetPickingGizmo(UObject* _Obj) { pickingGizmo = _Obj; };
};
