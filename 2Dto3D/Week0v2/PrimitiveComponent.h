#pragma once
#include "SceneComponent.h"
class UPrimitiveComponent : public USceneComponent
{
public:
	UPrimitiveComponent();
	~UPrimitiveComponent();

	virtual void		Initialize(UWorld* _World)	override;
	virtual void		Update(double deltaTime)		override;
	virtual void		Release()					override;
};

