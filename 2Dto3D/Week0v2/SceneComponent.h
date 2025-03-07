#pragma once
#include "Define.h"
#include "Object.h"

class USceneComponent : public UObject
{
public:
	FVector RelativeLocation;
	FVector RelativeRotation;
	FVector RelativeScale3D;
};

class UPrimitiveComponent : public USceneComponent
{
};

class UCubeComp : public UPrimitiveComponent
{
};

class USphereComp : public UPrimitiveComponent
{
};