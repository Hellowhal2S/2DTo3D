#pragma once
#include "SceneComponent.h"
class LocalGizmoComponent : public USceneComponent
{
public:
	LocalGizmoComponent();
	virtual				~LocalGizmoComponent();

	virtual void		Initialize();
	virtual void		Update(double deltaTime);
	virtual void		Release();
};

