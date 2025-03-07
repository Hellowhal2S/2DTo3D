#pragma once
#include "Define.h"
class UObject
{
public:
						UObject();
	virtual				~UObject();

	virtual void		Initialize();
	virtual void		Update();
	virtual void		Release();
};	

