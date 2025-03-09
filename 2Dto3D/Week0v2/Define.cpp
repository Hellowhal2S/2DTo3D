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
    float pitch = XMConvertToRadians(rotation.x);  // Pitch (X�� ȸ��)
    float yaw = XMConvertToRadians(rotation.y);    // Yaw (Y�� ȸ��)
    float roll = XMConvertToRadians(rotation.z);   // Roll (Z�� ȸ��)

    XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

    // ���� ���͸� XMMATRIX�� �����Ͽ� ȸ���� ���� ���ϱ�
    XMVECTOR vec = XMVectorSet(origin.x, origin.y, origin.z, 0.0f); // ���͸� XMVECTOR�� ��ȯ
    XMVECTOR rotatedVec = XMVector3TransformNormal(vec, rotationMatrix); // ȸ�� ��ȯ

    // ��� ���͸� FVector�� ��ȯ�Ͽ� ��ȯ
    XMFLOAT3 result;
    XMStoreFloat3(&result, rotatedVec);

    return FVector(result.x, result.y, result.z);
}