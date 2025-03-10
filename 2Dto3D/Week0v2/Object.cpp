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
    float pitch = JungleMath::DegToRad(m_Rotation.x);
    float yaw = JungleMath::DegToRad(m_Rotation.y);
    //float pitch = m_Rotation.x;
    //float yaw = m_Rotation.y;

    return FVector(
        cosf(pitch) * sinf(yaw),
        sinf(pitch),
        cosf(pitch) * cosf(yaw)
    ).Normalize();
}

FVector UObject::GetRightVector()
{
    float yaw = JungleMath::DegToRad(m_Rotation.y);
    return FVector(
        cosf(yaw), 0, -sinf(yaw)
    ).Normalize();
}

FVector UObject::GetUpVector()
{
    FVector Up = FVector(0.f, 1.f, 0.0f);
    Up = JungleMath::FVectorRotate(Up, m_Rotation);
    return Up;
}
