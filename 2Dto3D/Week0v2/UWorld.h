#pragma once
#include "AActor.h"
#include "ContainerDefs.h"
#include "ULevel.h"
class UWorld {
private:
	// 모든 액터들
	TArray<AActor*> Actors;
	// 현재 활성 레벨
	ULevel* PersistentLevel;

public:
	UWorld();

	template<class T>
	T* SpawnActor(UClass* Class, Fvector, Loc)

};