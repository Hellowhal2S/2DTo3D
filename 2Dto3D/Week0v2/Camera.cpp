#include "Camera.h"
#include <cmath>

UCamera::UCamera(){   }

UCamera::~UCamera() {}

void UCamera::Initialize()
{
    Yaw= 0.0f;
    Pitch = 0.0f;
    Forward = FVector(1.0f, 0.0f, 0.0f);
    Right = FVector(0.0f, 1.0f, 0.0f);
    Up = FVector(0.0f, 0.0f, 1.0f);
    RelativeLocation = FVector(-5.0f, 0.0f, 0.0f); // 초기 위치 설정
    UpdateVectors();
}

void UCamera::Update()
{
   
}

void UCamera::Release()
{
   
}

void UCamera::SetProjection(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
    float fovRadians = fovDegrees * (3.14159265359f / 180.0f);
    float yScale = 1.0f / tanf(fovRadians / 2);
    float xScale = yScale / aspectRatio;
    float zScale = farZ / (farZ - nearZ);

    ProjectionMatrix = { {
        { xScale, 0,     0,          0 },
        { 0,     yScale, 0,          0 },
        { 0,     0,     zScale,      1 },
        { 0,     0,     -nearZ * zScale, 0 }
    } };
}

void UCamera::MoveForward(float delta)
{
    RelativeLocation = RelativeLocation + (Forward * delta);
}

void UCamera::MoveRight(float delta)
{
    RelativeLocation = RelativeLocation + (Right * delta);
}

void UCamera::Rotate(float yawDelta, float pitchDelta)
{
    Yaw += yawDelta;
    Pitch += pitchDelta;

    // Pitch 제한 (-89 ~ 89도)
    if (Pitch > 89.0f) Pitch = 89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;

    UpdateVectors();
}

void UCamera::UpdateVectors()
{
    constexpr float DegToRad = 3.14159265359f / 180.0f;

    float radYaw = Yaw * DegToRad;
    float radPitch = Pitch * DegToRad;

    // Forward 벡터 계산 (Yaw 기준 Z축 회전)
    Forward.x = cosf(radYaw) * cosf(radPitch);
    Forward.y = sinf(radYaw) * cosf(radPitch);
    Forward.z = sinf(radPitch);
    Forward = Forward.Normalize();

    // 월드 업 벡터 (Z축 기준 Up)
    FVector worldUp(0.0f, 0.0f, 1.0f);

    // Right 벡터 (월드 업 벡터와 Forward 벡터의 외적)
    Right = worldUp.Cross(Forward).Normalize();

    // Up 벡터 (Forward와 Right의 외적)
    Up = Forward.Cross(Right).Normalize();
}




FMatrix UCamera::GetViewMatrix() const
{
    FVector target = RelativeLocation + Forward;

    FVector zAxis = (target- RelativeLocation).Normalize();
    FVector xAxis = Up.Cross(zAxis).Normalize();
    FVector yAxis = zAxis.Cross(xAxis);

    return { {
        { xAxis.x, yAxis.x, zAxis.x, 0 },
        { xAxis.y, yAxis.y, zAxis.y, 0 },
        { xAxis.z, yAxis.z, zAxis.z, 0 },
        { -xAxis.Dot(RelativeLocation), -yAxis.Dot(RelativeLocation), -zAxis.Dot(RelativeLocation), 1 }
    } };
}

FMatrix UCamera::GetProjectionMatrix() const
{
    return ProjectionMatrix;
}
void UCamera::LookAt(const FVector& target)
{

    Forward = (target - RelativeLocation).Normalize();

    
    Right = FVector(0.0f, 0.0f, 1.0f).Cross(Forward).Normalize();

    
    Up = Forward.Cross(Right).Normalize();
}

