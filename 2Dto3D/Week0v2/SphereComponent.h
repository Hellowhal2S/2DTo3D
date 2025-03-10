#pragma once

#include "PrimitiveComponent.h"

class USphereComp :public UPrimitiveComponent
{
public:
	USphereComp();
	~USphereComp();

	virtual void		Initialize()	override;
	virtual void		Update(double deltaTime)		override;
	virtual void		Release()					override;
	virtual	void		Render()					override;

};
