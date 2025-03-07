#pragma once
#include "SceneComponent.h"
class UCameraComponent :
    public USceneComponent
{
public:
	UCameraComponent();
	~UCameraComponent();

	virtual void		Initialize(UWorld* _World)	override;
	virtual void		Update(double deltaTime)		override;
	virtual void		Release()					override;

	void				Input();
	void				MoveForward(float _Value);
	void				MoveRight(float _Value);
};

