#include "JungleMath.h"

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
    // 스케일 행렬
    FMatrix Scale = FMatrix::Identity;
    Scale.M[0][0] = scale.x;
    Scale.M[1][1] = scale.y;
    Scale.M[2][2] = scale.z;

    // 회전 행렬 (Yaw-Pitch-Roll)
    float cosX = cos(rotation.x), sinX = sin(rotation.x);
    float cosY = cos(rotation.y), sinY = sin(rotation.y);
    float cosZ = cos(rotation.z), sinZ = sin(rotation.z);

    FMatrix RotationX = FMatrix::Identity;
    RotationX.M[1][1] = cosX; RotationX.M[1][2] = -sinX;
    RotationX.M[2][1] = sinX; RotationX.M[2][2] = cosX;

    FMatrix RotationY = FMatrix::Identity;
    RotationY.M[0][0] = cosY; RotationY.M[0][2] = sinY;
    RotationY.M[2][0] = -sinY; RotationY.M[2][2] = cosY;

    FMatrix RotationZ = FMatrix::Identity;
    RotationZ.M[0][0] = cosZ; RotationZ.M[0][1] = -sinZ;
    RotationZ.M[1][0] = sinZ; RotationZ.M[1][1] = cosZ;

    FMatrix Rotation = RotationZ * RotationY * RotationX;

    // 이동 행렬
    FMatrix Translation = FMatrix::Identity;
    Translation.M[3][0] = translation.x;
    Translation.M[3][1] = translation.y;
    Translation.M[3][2] = translation.z;

    // 최종 모델 행렬 = 이동 * 회전 * 스케일
    return Translation * Rotation * Scale;
}

FMatrix JungleMath::CreateViewMatrix(FVector eye, FVector target, FVector up)
{
    FVector zAxis = (target - eye).Normalize();
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
    Projection.M[2][2] = -(farPlane + nearPlane) / depth;
    Projection.M[2][3] = -1.0f;
    Projection.M[3][2] = -(2.0f * farPlane * nearPlane) / depth;

    return Projection;
}
