#pragma once
#include "Define.h"
class UWorld;
class UObject
{
public:
						UObject();
	virtual				~UObject();

	virtual void		Initialize(UWorld* _World);
	virtual void		Update(double deltaTime);
	virtual void		Release();

protected:
	FVector				m_Location;
	FVector				m_Rotation;
	FVector				m_Scale;

	UWorld*				m_World;
public:
	inline FVector				GetLocation() { return m_Location; }
	inline FVector				GetRotation() { return m_Rotation; }
	inline FVector				GetScale()    {	return m_Scale;    }

};	

