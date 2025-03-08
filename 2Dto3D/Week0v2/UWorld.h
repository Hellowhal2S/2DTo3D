#pragma once
#include "ContainerDefs.h"
#include "UObject.h"
#include "UCameraComponent.h"
#include "Define.h"
class UWorld {
public :
	UWorld();
	virtual ~UWorld();
private:
	// ��� ������Ʈ��
	TArray<TDoubleLinkedList<UObject*>> ObjectLists;
	// ���� Ȱ�� ����
	//ULevel* PersistentLevel;

public:
	void InitWorld();
	void UpdateWorld(double deltaTime);
	void RenderWorld();
	void ReleaseWorld();
	UObject* GetCamera() {return ObjectLists[OBJ_CAMERA].front();}
};