#pragma once
#include "ContainerDefs.h"
//#include "UObject.h"
//#include "UCameraComponent.h"
#include "Define.h"
class UObject;
class UCameraComponent;
class USphereComp;

class UWorld {
public :
	UWorld();
	virtual ~UWorld();
	UCameraComponent* mainCamera;
private:
	// ��� ������Ʈ��
	TArray<TDoubleLinkedList<UObject*>> ObjectLists;
	TArray<UObject* > PrimitiveList;
	// ���� Ȱ�� ����
	//ULevel* PersistentLevel;

public:
	void InitWorld();
	void UpdateWorld(double deltaTime);
	void RenderWorld();
	void ReleaseWorld();
	UObject* GetCamera() {return ObjectLists[OBJ_CAMERA].front();}

	TArray<TDoubleLinkedList<UObject*>> GetObjectLists();
	TArray<UObject* > GetPrimitiveList();
};