#pragma once
#include "UEContainerAliases.h"

class UObject
{
public:
	UObject();
	virtual ~UObject();

public:
	virtual void Initialize();
	virtual void Update();
	virtual void Release();

public:
	uint32 UUID;
	uint32 InternalIndex; // Index of GUObjectArray
};

TArray<UObject*> GUObjectArray;