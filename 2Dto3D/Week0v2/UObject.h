#pragma once
class UObject
{
	UObject();
	virtual ~UObject();

	virtual void Initialize();
	virtual void Update();
	virtual void Release();
};

