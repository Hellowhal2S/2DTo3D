#pragma once
#include "AActor.h"
#include "ContainerDefs.h"
#include "ULevel.h"
class UWorld {
private:
	// ��� ���͵�
	TArray<AActor*> Actors;
	// ���� Ȱ�� ����
	ULevel* PersistentLevel;

public:
	UWorld();

	template<class T>
	T* SpawnActor(UClass* Class, Fvector, Loc)

};