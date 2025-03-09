#pragma once
#include "Define.h"
class UObject
{
public:
						UObject();
				~UObject();

	virtual void		Initialize();
	virtual void		Update();
	virtual void		Release();
	uint32 UUID;
	uint32 InternalIndex;
};	

