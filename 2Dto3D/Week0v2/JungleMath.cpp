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
    // 스케일 행렬
    FMatrix Scale = FMatrix::Identity;
    Scale.M[0][0] = scale.x;
    Scale.M[1][1] = scale.y;
    Scale.M[2][2] = scale.z;

    // 회전 행렬 (Yaw-Pitch-Roll)
    float x = DegToRad(rotation.x);
    float y = DegToRad(rotation.y);
    float z = DegToRad(rotation.z);

    //float cosX = cos(rotation.x), sinX = sin(rotation.x);
    //float cosY = cos(rotation.y), sinY = sin(rotation.y);
    //float cosZ = cos(rotation.z), sinZ = sin(rotation.z);
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

    FMatrix Rotation = RotationZ * RotationY * RotationX;

    // 이동 행렬
    FMatrix Translation = FMatrix::Identity;
    Translation.M[3][0] = translation.x;
    Translation.M[3][1] = translation.y;
    Translation.M[3][2] = translation.z;
    Translation.M[3][3] = 1.0f;

    // 최종 모델 행렬 = 이동 * 회전 * 스케일
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
    Projection.M[2][2] = farPlane / depth;  // 수정된 부분
    Projection.M[2][3] = 1.0f;              // 수정된 부분 (RH는 -1.0f였음)
    Projection.M[3][2] = -(nearPlane * farPlane) / depth;
    Projection.M[3][3] = 1.0f;

    return Projection;
}

FVector JungleMath::FVectorRotateCamera(FVector& origin, const FVector& rotation)
{
    float pitch = XMConvertToRadians(rotation.x);  // Pitch (X축 회전)
    float yaw = XMConvertToRadians(rotation.y);    // Yaw (Y축 회전)
    float roll = XMConvertToRadians(rotation.z);   // Roll (Z축 회전)

    //float pitch = rotation.x;  // Pitch (X축 회전)
    //float yaw = rotation.y;   // Yaw (Y축 회전)
    //    float roll = rotation.z;   // Roll (Z축 회전)
    XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

    // 현재 벡터를 XMMATRIX와 결합하여 회전된 벡터 구하기
    XMVECTOR vec = XMVectorSet(origin.x, origin.y, origin.z, 0.0f); // 벡터를 XMVECTOR로 변환
    XMVECTOR rotatedVec = XMVector3TransformNormal(vec, rotationMatrix); // 회전 변환

    // 결과 벡터를 FVector로 변환하여 반환
    XMFLOAT3 result;
    XMStoreFloat3(&result, rotatedVec);

    return FVector(result.x, result.y, result.z);
}
FVector JungleMath::FVectorRotate(FVector& origin, const FVector& rotation)
{
    // 회전 값 (degree -> radian 변환)
    float pitch = rotation.x * XM_PI / 180.0f;
    float yaw = rotation.y * XM_PI / 180.0f;
    float roll = rotation.z * XM_PI / 180.0f;

    // 삼각함수 값 미리 계산
    float cosPitch = cosf(pitch), sinPitch = sinf(pitch);
    float cosYaw = cosf(yaw), sinYaw = sinf(yaw);
    float cosRoll = cosf(roll), sinRoll = sinf(roll);

    // 회전 행렬을 직접 적용하여 새 위치 계산
    FVector rotated;
    rotated.x = origin.x * (cosYaw * cosRoll) +
        origin.y * (cosYaw * sinRoll) +
        origin.z * (-sinYaw);

    rotated.y = origin.x * (sinPitch * sinYaw * cosRoll - cosPitch * sinRoll) +
        origin.y * (sinPitch * sinYaw * sinRoll + cosPitch * cosRoll) +
        origin.z * (sinPitch * cosYaw);

    rotated.z = origin.x * (cosPitch * sinYaw * cosRoll + sinPitch * sinRoll) +
        origin.y * (cosPitch * sinYaw * sinRoll - sinPitch * cosRoll) +
        origin.z * (cosPitch * cosYaw);

    return rotated;
}
float JungleMath::RadToDeg(float radian)
{
    return radian * (180.0f / PI);
}

float JungleMath::DegToRad(float degree)
{
    return degree * (PI / 180.0f);
}


