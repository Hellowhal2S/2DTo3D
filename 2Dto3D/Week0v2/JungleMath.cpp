#include "JungleMath.h"
#include <DirectXMath.h>

using namespace DirectX;
FVector4 JungleMath::ConvertV3ToV4(FVector vec3)
{
	FVector4 newVec4;
	newVec4.x = vec3.x;
	newVec4.y = vec3.y;
	newVec4.z = vec3.z;
	return newVec4;
}


FMatrix JungleMath::CreateModelMatrix(FVector translation, FVector rotation, FVector scale)
{
    // 1. 스케일 행렬
    FMatrix Scale = FMatrix::Identity;
    Scale.M[0][0] = scale.x;
    Scale.M[1][1] = scale.y;
    Scale.M[2][2] = scale.z;

    // 2. 회전 행렬 (Yaw-Pitch-Roll 순서, LH 좌표계)
    float x = DegToRad(rotation.x);
    float y = DegToRad(rotation.y);
    float z = DegToRad(rotation.z);

    float cosX = cos(x), sinX = sin(x);
    float cosY = cos(y), sinY = sin(y);
    float cosZ = cos(z), sinZ = sin(z);

    FMatrix RotationX = FMatrix::Identity;
    RotationX.M[1][1] = cosX; RotationX.M[1][2] = -sinX;
    RotationX.M[2][1] = sinX; RotationX.M[2][2] = cosX;

    FMatrix RotationY = FMatrix::Identity;
    RotationY.M[0][0] = cosY; RotationY.M[0][2] = sinY;
    RotationY.M[2][0] = -sinY; RotationY.M[2][2] = cosY;

    FMatrix RotationZ = FMatrix::Identity;
    RotationZ.M[0][0] = cosZ; RotationZ.M[0][1] = -sinZ;
    RotationZ.M[1][0] = sinZ; RotationZ.M[1][1] = cosZ;

    // Yaw-Pitch-Roll 적용 순서 -> Z * Y * X
    FMatrix Rotation = RotationZ * RotationY * RotationX;

    // 3. 이동 행렬
    FMatrix Translation = FMatrix::Identity;
    Translation.M[3][0] = translation.x;
    Translation.M[3][1] = translation.y;
    Translation.M[3][2] = translation.z;

    return Scale * Rotation * Translation;
}

FMatrix JungleMath::CreateViewMatrix(FVector eye, FVector target, FVector up)
{
    FVector zAxis = (target - eye).Normalize();  // DirectX는 LH이므로 -z가 아니라 +z 사용
    FVector xAxis = (up.Cross(zAxis)).Normalize();
    FVector yAxis = zAxis.Cross(xAxis);

    FMatrix View;
    View.M[0][0] = xAxis.x; View.M[0][1] = yAxis.x; View.M[0][2] = zAxis.x; View.M[0][3] = 0;
    View.M[1][0] = xAxis.y; View.M[1][1] = yAxis.y; View.M[1][2] = zAxis.y; View.M[1][3] = 0;
    View.M[2][0] = xAxis.z; View.M[2][1] = yAxis.z; View.M[2][2] = zAxis.z; View.M[2][3] = 0;
    View.M[3][0] = -xAxis.Dot(eye);
    View.M[3][1] = -yAxis.Dot(eye);
    View.M[3][2] = -zAxis.Dot(eye);
    View.M[3][3] = 1;

    return View;
}

FMatrix JungleMath::CreateProjectionMatrix(float fov, float aspect, float nearPlane, float farPlane)
{
    float tanHalfFOV = tan(fov / 2.0f);
    float depth = farPlane - nearPlane;

    FMatrix Projection = {};
    Projection.M[0][0] = 1.0f / (aspect * tanHalfFOV);
    Projection.M[1][1] = 1.0f / tanHalfFOV;
    Projection.M[2][2] = farPlane / depth;
    Projection.M[2][3] = 1.0f;
    Projection.M[3][2] = -(nearPlane * farPlane) / depth;
    Projection.M[3][3] = 0.0f;  // ✅ 수정: 원래는 1.0f가 아니라 0.0f 여야 함.

    return Projection;
}

//FVector JungleMath::FVectorRotate(FVector& origin, const FVector& rotation)
//{
//    // 회전 값 (degree -> radian 변환)
//    float pitch = XMConvertToRadians(rotation.x); // X축 (Pitch)
//    float yaw = XMConvertToRadians(rotation.y); // Y축 (Yaw)
//    float roll = XMConvertToRadians(rotation.z); // Z축 (Roll)
//
//    // Yaw-Pitch-Roll 회전 행렬 생성
//    XMMATRIX rotMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
//
//    // 원본 벡터를 XMVECTOR로 변환
//    XMVECTOR vec = XMVectorSet(origin.x, origin.y, origin.z, 1.0f);
//
//    // 회전 적용
//    vec = XMVector3Transform(vec, rotMatrix);
//
//    // 결과를 FVector로 변환하여 반환
//    FVector rotated;
//    rotated.x = XMVectorGetX(vec);
//    rotated.y = XMVectorGetY(vec);
//    rotated.z = XMVectorGetZ(vec);
//
//    return rotated;
//}

FVector JungleMath::FVectorRotate(FVector& origin, const FVector& rotation)
{
    // 회전 각도를 라디안으로 변환
    float pitch = DegToRad(rotation.x) * 0.5f;
    float yaw = DegToRad(rotation.y) * 0.5f;
    float roll = DegToRad(rotation.z) * 0.5f;

    // 쿼터니언 생성 (Yaw-Pitch-Roll 순서)
    float cosX = cosf(pitch), sinX = sinf(pitch);
    float cosY = cosf(yaw), sinY = sinf(yaw);
    float cosZ = cosf(roll), sinZ = sinf(roll);

    FQuat quaternion;
    quaternion.w = cosX * cosY * cosZ + sinX * sinY * sinZ;
    quaternion.x = sinX * cosY * cosZ - cosX * sinY * sinZ;
    quaternion.y = cosX * sinY * cosZ + sinX * cosY * sinZ;
    quaternion.z = cosX * cosY * sinZ - sinX * sinY * cosZ;

    // 쿼터니언을 이용해 벡터 회전 적용
    return quaternion.RotateVector(origin);
}


float JungleMath::RadToDeg(float radian)
{
    return radian * (180.0f / PI);
}

float JungleMath::DegToRad(float degree)
{
    return degree * (PI / 180.0f);
}


