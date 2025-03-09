#pragma once
#include "UObject.h"
#include "VectorUtils.h"
class USceneComponent : public UObject
{
public:
	USceneComponent() {};
	virtual ~USceneComponent() {};
	
	FVector RelativeLocation;
	FVector RelativeRotation;
	FVector RelativeScale3D;

	FVector GetUpVector();
	FVector GetForwardVector();
	FVector GetRightVector();
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