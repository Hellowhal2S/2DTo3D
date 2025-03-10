#pragma once
#include "Define.h"
#include "EngineLoop.h"
#include "World.h"
extern FEngineLoop GEngineLoop;

class UWorld;
class UObject
{
public:
						UObject();
	virtual				~UObject();

	virtual void		Initialize();
	virtual void		Update(double deltaTime);
	virtual void		Release();
	virtual void		Render();
	FVector				GetForwardVector();
	FVector				GetRightVector();
	FVector				GetCameraForwardVector();
	FVector				GetCameraRightVector();
	FVector				GetUpVector();
	FVector				GetLocation();
	void				SetLocation(FVector _newLoc);
protected:
	FVector				m_Location;
	FVector				m_Rotation;
	FVector				m_Scale;

	UWorld*				m_World;
public:
	inline FVector				GetRotation() { return m_Rotation; }
	inline FVector				GetScale()    {	return m_Scale;    }
	inline void					SetRotation(FVector _newRot) { m_Rotation = _newRot; }
	inline void					SetScale(FVector _newScale) { m_Scale = _newScale; }

	inline UWorld*				GetWorld() { return GEngineLoop.GetWorld(); }
	inline FEngineLoop&			GetEngine() { return GEngineLoop; }

};	

