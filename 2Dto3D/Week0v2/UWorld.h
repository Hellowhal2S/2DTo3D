#pragma once
#include "ContainerDefs.h"
//#include "UObject.h"
//#include "UCameraComponent.h"
#include "Define.h"
#include "PickingSystem.h"

class UObject;
class UCameraComponent;
class USphereComp;
class USceneComponent;
class UWorld {
public :
	UWorld();
	virtual ~UWorld();
	UCameraComponent* mainCamera;
	static UWorld* myWorld;
	static USceneComponent* pickingObject;

private:
	// 모든 오브젝트들
	TArray<TArray<UObject*>> ObjectLists;
	TArray<UObject* > PrimitiveList;
	// 현재 활성 레벨
	//ULevel* PersistentLevel;
	
	//UPickingSystem pickingSystem;

public:
	void InitWorld();
	void UpdateWorld(double deltaTime);
	void RenderWorld();
	void ReleaseWorld();
	UObject* GetCamera() {return ObjectLists[OBJ_CAMERA].front();}

	TArray<TArray<UObject*>> GetObjectLists();
	TArray<UObject* > GetPrimitiveList();
	
	USceneComponent* GetPickingObject();
	void SetPickingObject(USceneComponent* obj);
};