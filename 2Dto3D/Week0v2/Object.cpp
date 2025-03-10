#include "Object.h"
#include "JungleMath.h"


UObject::UObject():m_Location(FVector(0.f,0.f,0.f)),m_Rotation(FVector(0.f,0.f,0.f)),m_Scale(FVector(1.f,1.f,1.f))
{
}

UObject::~UObject()
{
}



void UObject::Initialize()
{
}

void UObject::Update(double deltaTime)
{
}



void UObject::Release()
{
}
void UObject::Render()
{
}
//
FVector UObject::GetForwardVector()
{
	FVector Forward = FVector(0.f,0.f,1.0f);
	Forward = JungleMath::FVectorRotate(Forward, m_Rotation * -1);
	return Forward;
}

FVector UObject::GetRightVector()
{
	FVector Right = FVector(1.f, 0.f, 0.0f);
	Right = JungleMath::FVectorRotate(Right, m_Rotation * -1);
	return Right;
}

//FVector UObject::GetForwardVector()
//{
//	FVector Forward = FVector(0.f, 0.f, 1.0f);
//	Forward = JungleMath::FVectorRotate(Forward, m_Rotation);
//	return Forward;
//}
//
//FVector UObject::GetRightVector()
//{
//	FVector Right = FVector(1.f, 0.f, 0.0f);
//	Right = JungleMath::FVectorRotate(Right, m_Rotation);
//	return Right;
//}


FVector UObject::GetUpVector()
{
	FVector Up = FVector(0.f, 1.f, 0.0f);
	Up = JungleMath::FVectorRotate(Up, m_Rotation);
	return Up;
}

FVector UObject::GetLocation()
{
    FVector result = m_Location;
    result.z *= -1;
    return m_Location;
}

void UObject::SetLocation(FVector _newLoc)
{
    FVector newLoc = _newLoc;
    newLoc.z *= -1;
    m_Location = _newLoc;
}
