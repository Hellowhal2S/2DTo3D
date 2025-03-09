#pragma once
#include "SceneComponent.h"
class LocalGizmoComponent : public USceneComponent
{
public:
	LocalGizmoComponent();
	virtual				~LocalGizmoComponent();

	virtual void		Initialize(UWorld* _World);
	virtual void		Update(double deltaTime);
	virtual void		Release();
};

