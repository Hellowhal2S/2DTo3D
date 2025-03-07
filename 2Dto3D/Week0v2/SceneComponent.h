#pragma once
#include "Object.h"
class USceneComponent : public UObject
{
public:
	USceneComponent();
	~USceneComponent();

	virtual void		Initialize(UWorld* _World)	override;
	virtual void		Update(double deltaTime)		override;
	virtual void		Release()					override;
public:
	FVector RelativeLocation;
	FVector RelativeRotation;
	FVector RelativeScale3D;
};

