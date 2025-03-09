#include "Object.h"
#include "JungleMath.h"
UObject::UObject():m_Location(FVector(0.f,0.f,0.f)),m_Rotation(FVector(0.f,0.f,0.f)),m_Scale(FVector(1.f,1.f,1.f))
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

FVector UObject::GetForwardVector()
{
	FVector Forward = FVector(0.f,0.f,1.0f);
	Forward = JungleMath::FVectorRotate(Forward, m_Rotation);
	return Forward;
}

FVector UObject::GetRightVector()
{
	FVector Right = FVector(1.f, 0.f, 0.0f);
	Right = JungleMath::FVectorRotate(Right, m_Rotation);
	return Right;
}

FVector UObject::GetUpVector()
{
	FVector Up = FVector(0.f, 1.f, 0.0f);
	Up = JungleMath::FVectorRotate(Up, m_Rotation);
	return Up;
}
