#pragma once
#include "Define.h"
#include "UEContainerAliases.h"

class UObject;
class UWorld
{
public:
	UWorld();
	~UWorld();

public:
	void Initialize();
	void Release();
	void Update();
	void Render();

private:
	/** Array of all list of all objects in this world */
	TArray<TDoubleLinkedList<UObject*>> pObjectList;

public:
	TDoubleLinkedList<UObject*>* GetObjectList(OBJECTLIST index) { return &pObjectList[index]; }
};