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
    RelativeLocation = FVector(-5.0f, 0.0f, 0.0f); // �ʱ� ��ġ ����
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

    // Pitch ���� (-89 ~ 89��)
    if (Pitch > 89.0f) Pitch = 89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;

    UpdateVectors();
}

void UCamera::UpdateVectors()
{
    constexpr float DegToRad = 3.14159265359f / 180.0f;

    float radYaw = Yaw * DegToRad;
    float radPitch = Pitch * DegToRad;

    // Forward ���� ��� (Yaw ���� Z�� ȸ��)
    Forward.x = cosf(radYaw) * cosf(radPitch);
    Forward.y = sinf(radYaw) * cosf(radPitch);
    Forward.z = sinf(radPitch);
    Forward = Forward.Normalize();

    // ���� �� ���� (Z�� ���� Up)
    FVector worldUp(0.0f, 0.0f, 1.0f);

    // Right ���� (���� �� ���Ϳ� Forward ������ ����)
    Right = worldUp.Cross(Forward).Normalize();

    // Up ���� (Forward�� Right�� ����)
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

