#include "Object.h"

#include "JungleMath.h"

UObject::UObject() :m_Location(FVector(0.f, 0.f, 0.f)), m_Rotation(FVector(0.f, 0.f, 0.f)), m_Scale(FVector(1.f, 1.f, 1.f))
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

FVector UObject::GetForwardVector()
{
    FVector Forward = FVector(1.f, 0.f, 0.f);
    Forward = JungleMath::FVectorRotateCamera(Forward, m_Rotation);
    return Forward;
    //float pitch = JungleMath::DegToRad(m_Rotation.y);
    //float yaw = JungleMath::DegToRad(m_Rotation.z);

    //return FVector(
    //    cosf(pitch) * cosf(yaw),
    //    cosf(pitch) * sinf(yaw),
    //    sinf(pitch)
    //).Normalize();
}

FVector UObject::GetRightVector()
{
	FVector Right = FVector(0.f, 1.f, 0.f);
	Right = JungleMath::FVectorRotateCamera(Right, m_Rotation);
    return Right;
  //  float yaw = JungleMath::DegToRad(m_Rotation.z);

  //  return FVector(
		//cosf(yaw), sinf(yaw), 0.f
  //  ).Normalize();
}

FVector UObject::GetUpVector()
{
    FVector Up = FVector(0.f, 0.f, 1.0f);
    Up = JungleMath::FVectorRotateCamera(Up, m_Rotation);
    return Up;
}
