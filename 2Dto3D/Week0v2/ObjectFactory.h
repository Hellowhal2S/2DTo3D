#pragma once

#include "Object.h"

class FObjectFactory
{
public:
	template<class T>
	static UObject* ConstructObject()
	{
		UObject* obj = new T;
		obj->Initialize();
		return obj;
	}
};
