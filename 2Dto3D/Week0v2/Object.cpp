#include "Object.h"

UObject::UObject()
{
}

UObject::~UObject()
{
}



void UObject::Initialize(UWorld* _World)
{
	m_World = _World;
}

void UObject::Update(double deltaTime)
{
}



void UObject::Release()
{
}
