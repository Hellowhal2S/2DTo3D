#pragma once
#include "Object.h"
class USceneComponent : public UObject
{
public:
	USceneComponent();
	~USceneComponent();

public:
	FVector RelativeLocation;
	FVector RelativeRotation;
	FVector RelativeScale3D;
};

