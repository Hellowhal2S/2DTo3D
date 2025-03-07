#pragma once
#include "PrimitiveComponent.h"
class USphereComp :public UPrimitiveComponent
{
public:
	USphereComp();
	~USphereComp();

	virtual void		Initialize(UWorld* _World)	override;
	virtual void		Update(double deltaTime)		override;
	virtual void		Release()					override;
};

