#pragma once
#include "Define.h"
class UObject
{
						UObject();
	virtual				~UObject();

	virtual void		Initialize();
	virtual void		Update();
	virtual void		Release();

protected:
	FVector				m_Location;
	FVector				m_Rotation;
	FVector				m_Scale;

public:
	inline FVector				GetLocation() { return m_Location; }
	inline FVector				GetRotation() { return m_Rotation; }
	inline FVector				GetScale()    {	return m_Scale;    }

};	

