#pragma once
#include "ContainerDefs.h"
//#include "UObject.h"
//#include "UCameraComponent.h"
#include "Define.h"

class UObject;
class UCameraComponent;
class USphereComp;
class USceneComponent;
class UPickingSystem;
class UWorld {
public :
	UWorld();
	virtual ~UWorld();
	UCameraComponent* mainCamera;
	static UWorld* myWorld;
	static USceneComponent* pickingObject;
	UPickingSystem* pickingSystem;

private:
	// ��� ������Ʈ��
	TArray<TArray<UObject*>> ObjectLists;
	TArray<UObject* > PrimitiveList;
	// ���� Ȱ�� ����
	//ULevel* PersistentLevel;
	
	

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