#pragma once
#include "PrimitiveComponent.h"
class UCubeComp : public UPrimitiveComponent
{
public:
	UCubeComp();
	~UCubeComp();

	virtual void		Initialize(UWorld* _World)	override;
	virtual void		Update(double deltaTime)		override;
	virtual void		Release()					override;
};

