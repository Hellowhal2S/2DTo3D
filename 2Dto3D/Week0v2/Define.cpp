#include "Define.h"

FVector4 Utils::ConvertV3ToV4(FVector vec3)
{
	FVector4 newVec4;
	newVec4.x = vec3.x;
	newVec4.y = vec3.y;
	newVec4.z = vec3.z;
	return newVec4;
}

FVector Utils::FVectorRotate(FVector& origin, const FVector& rotation) {
    float pitch = XMConvertToRadians(rotation.x);  // Pitch (X축 회전)
    float yaw = XMConvertToRadians(rotation.y);    // Yaw (Y축 회전)
    float roll = XMConvertToRadians(rotation.z);   // Roll (Z축 회전)

    XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

    // 현재 벡터를 XMMATRIX와 결합하여 회전된 벡터 구하기
    XMVECTOR vec = XMVectorSet(origin.x, origin.y, origin.z, 0.0f); // 벡터를 XMVECTOR로 변환
    XMVECTOR rotatedVec = XMVector3TransformNormal(vec, rotationMatrix); // 회전 변환

    // 결과 벡터를 FVector로 변환하여 반환
    XMFLOAT3 result;
    XMStoreFloat3(&result, rotatedVec);

    return FVector(result.x, result.y, result.z);
}