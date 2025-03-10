#pragma once

#include "PrimitiveComponent.h"

class UArrowComp : public UPrimitiveComponent
{
public:
	UArrowComp();
	~UArrowComp();

	virtual void		Initialize()				override;
	virtual void		Update(double deltaTime)	override;
	virtual void		Release()					override;
	virtual	void		Render()					override;
};
