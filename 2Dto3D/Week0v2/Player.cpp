#include "Player.h"
#include <Windows.h>
#include "GraphicDevice.h"
#include "World.h"
#include "Define.h"
#include "EngineLoop.h"
#include "PrimitiveComponent.h"
#include "JungleMath.h"
#include <DirectXMath.h>
#include "ArrowComp.h"
using namespace DirectX;

UPlayer::UPlayer()
{
}

UPlayer::~UPlayer()
{
}

void UPlayer::Initialize()
{
	Super::Initialize();
}

void UPlayer::Update(double deltaTime)
{
	Input();
}

void UPlayer::Release()
{
}

void UPlayer::Input()
{

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 )
	{
		if (!bLeftMouseDown) {
			bLeftMouseDown = true;

			UE_LOG(LogLevel::Warning, "Down");

			POINT mousePos;
			GetCursorPos(&mousePos);
			GetCursorPos(&m_LastMousePos);


			ScreenToClient(GEngineLoop.hWnd, &mousePos);

			FVector rayOrigin;
			FVector rayDir;
			ScreenToRay(mousePos.x, mousePos.y, GEngineLoop.View, GEngineLoop.Projection, rayOrigin, rayDir);
			UObject* Possible = nullptr;
			//for (auto iter : GetWorld()->GetObjectArr())
			//{
			//	UPrimitiveComponent* pObj = dynamic_cast<UPrimitiveComponent*>(iter);
			//	if (pObj)
			//	{
			//		FVector BoxMin, BoxMax;
			//		GetBoxMinMax(pObj->GetLocation(), pObj->GetScale(), pObj->GetRotation(), BoxMin, BoxMax);
			//		if (RayIntersectsBox(rayOrigin, rayDir, BoxMin, BoxMax, pObj->GetRotation()))
			//		{
			//			if (!Possible)
			//				Possible = pObj;
			//			else if (Possible->GetLocation().Distance(rayOrigin) > (pObj->GetLocation().Distance(rayOrigin)))
			//				Possible = pObj;
			//		}
			//	}
			//};
			//if (Possible)
			//	GetWorld()->SetPickingObj(Possible);

			for (auto iter : GetWorld()->GetObjectArr())
			{
				UPrimitiveComponent* pObj = dynamic_cast<UPrimitiveComponent*>(iter);

				if (pObj && pObj->GetType() != "Arrow")
				{
					FVector BoxMin, BoxMax;
					float MaxScale = max(max(pObj->GetScale().x, pObj->GetScale().y), pObj->GetScale().z);
					if (RayIntersectsSphere(rayOrigin, rayDir, pObj->GetLocation(), MaxScale))
					{
						if (!Possible)
							Possible = pObj;
						else if (Possible->GetLocation().Distance(rayOrigin) > (pObj->GetLocation().Distance(rayOrigin)))
							Possible = pObj;
					}
				}
			}
			if (Possible)
				GetWorld()->SetPickingObj(Possible);
			if (GetWorld()->GetPickingObj()) {
				for (int i = 0;i < 3;++i)
				{
					UArrowComp* Arrow = static_cast<UArrowComp*>(GetWorld()->LocalGizmo[i]);
					float Scale = 0.0f;
					FVector DetectLoc;
					if (i == 0) {
						Scale = GetWorld()->LocalGizmo[0]->GetScale().x;
						DetectLoc = Arrow->GetLocation() + GetWorld()->GetPickingObj()->GetRightVector();
					}
					else if (i == 1) {
						Scale = GetWorld()->LocalGizmo[1]->GetScale().y;
						DetectLoc = Arrow->GetLocation() + GetWorld()->GetPickingObj()->GetUpVector();
					}
					else if (i == 2) {
						Scale = GetWorld()->LocalGizmo[2]->GetScale().z;
						DetectLoc = Arrow->GetLocation() + GetWorld()->GetPickingObj()->GetForwardVector();
					}
					if (RayIntersectsSphere(rayOrigin, rayDir, DetectLoc, 0.5f))
					{
						GetWorld()->SetPickingGizmo(GetWorld()->LocalGizmo[i]);
						UE_LOG(LogLevel::Display, "%d", Arrow->GetDir());
					}
				}
			}	
		}
		else
		{
			// ���콺 �̵��� ���
			if (GetWorld()->GetPickingObj() && GetWorld()->GetPickingGizmo()) {
				POINT currentMousePos;
				GetCursorPos(&currentMousePos);
				UObject* pObj = GetWorld()->GetPickingObj();
				// ���콺 �̵� ���� ���
				int32 deltaX = currentMousePos.x - m_LastMousePos.x;
				int32 deltaY = currentMousePos.y - m_LastMousePos.y;
				UArrowComp* Arrow = static_cast<UArrowComp*>(GetWorld()->GetPickingGizmo());
				switch (Arrow->GetDir())
				{
				case AD_X:
					pObj->AddLocation(pObj->GetRightVector() * deltaX * 0.01f);
					break;
				case AD_Y:
					pObj->AddLocation((pObj->GetUpVector() * deltaY * 0.01f) * -1 );
					break;
				case AD_Z:
					pObj->AddLocation(pObj->GetForwardVector() * deltaX*0.01f);
					break;
				default:
					break;
				}

				// ���ο� ���콺 ��ġ ����
				m_LastMousePos = currentMousePos;
			}
		}
	}
	else
	{
		if (bLeftMouseDown) {
				UE_LOG(LogLevel::Warning, "Up");
				bLeftMouseDown = false; // ���콺 ������ ��ư�� ���� ���� �ʱ�ȭ
				GetWorld()->SetPickingGizmo(nullptr);
			
		}
	}
}


bool UPlayer::RayIntersectsBox(const FVector& rayOrigin, const FVector& rayDir, const FVector& boxMin, const FVector& boxMax, const FVector& boxRotation)
{
	// 1. ȸ�� ��� ���� (FVector�� �־��� ȸ������ roll, pitch, yaw�� �ؼ��Ͽ� ȸ�� ����� ����)
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(boxRotation.x, boxRotation.y, boxRotation.z);

	// 2. ������ ������ ������ ���� ��ǥ��� ��ȯ
	XMVECTOR rayOriginVec = XMVectorSet(rayOrigin.x, rayOrigin.y, rayOrigin.z, 1.0f);
	XMVECTOR rayDirVec = XMVectorSet(rayDir.x, rayDir.y, rayDir.z, 0.0f);

	// ȸ�� ����� ����Ͽ� ���̸� ���� ��ǥ��� ��ȯ
	XMMATRIX inverseRotationMatrix = XMMatrixInverse(nullptr, rotationMatrix);
	rayOriginVec = XMVector3TransformCoord(rayOriginVec, inverseRotationMatrix);
	rayDirVec = XMVector3TransformNormal(rayDirVec, inverseRotationMatrix);

	// 3. ���� ��ǥ�迡�� ������ ������ ������ FVector�� ��ȯ
	FVector localRayOrigin(XMVectorGetX(rayOriginVec), XMVectorGetY(rayOriginVec), XMVectorGetZ(rayOriginVec));
	FVector localRayDir(XMVectorGetX(rayDirVec), XMVectorGetY(rayDirVec), XMVectorGetZ(rayDirVec));

	// 4. �ڽ��� ���� ���θ� ��� (AABB ���� �˻�)
	float tmin = (boxMin.x - localRayOrigin.x) / localRayDir.x;
	float tmax = (boxMax.x - localRayOrigin.x) / localRayDir.x;

	if (tmin > tmax) std::swap(tmin, tmax);

	float tymin = (boxMin.y - localRayOrigin.y) / localRayDir.y;
	float tymax = (boxMax.y - localRayOrigin.y) / localRayDir.y;

	if (tymin > tymax) std::swap(tymin, tymax);

	if (tmin > tymax || tymin > tmax)
		return false;

	if (tymin > tmin) tmin = tymin;
	if (tymax < tmax) tmax = tymax;

	float tzmin = (boxMin.z - localRayOrigin.z) / localRayDir.z;
	float tzmax = (boxMax.z - localRayOrigin.z) / localRayDir.z;

	if (tzmin > tzmax) std::swap(tzmin, tzmax);

	if (tmin > tzmax || tzmin > tmax)
		return false;

	if (tzmin > tmin) tmin = tzmin;
	if (tzmax < tmax) tmax = tzmax;

	// 5. ���̰� ������ü�� �����ϸ� true ��ȯ
	return true;
}

void UPlayer::GetBoxMinMax(const FVector& boxLocation, const FVector& boxScale, const FVector& boxRotation, FVector& boxMin, FVector& boxMax)
{
	// 1. �ڽ� ũ���� ���� (half-extents) ���
	FVector halfExtents = boxScale * 0.5f;

	// 2. ȸ�� ����� ���� (FVector�� ȸ�� ��ķ� ��ȯ)
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(boxRotation.x, boxRotation.y, boxRotation.z);

	// 3. �ڽ��� ���� ũ�⸦ ȸ����Ŵ
	XMVECTOR halfExtentsVec = XMVectorSet(halfExtents.x, halfExtents.y, halfExtents.z, 0.0f);
	XMVECTOR rotatedHalfExtentsVec = XMVector3TransformNormal(halfExtentsVec, rotationMatrix);

	// 4. ȸ���� ���� ũ�� ���� FVector�� ��ȯ
	FVector rotatedHalfExtents(
		XMVectorGetX(rotatedHalfExtentsVec),
		XMVectorGetY(rotatedHalfExtentsVec),
		XMVectorGetZ(rotatedHalfExtentsVec)
	);

	// 5. Box�� min, max �� ���
	boxMin = boxLocation - rotatedHalfExtents;
	boxMax = boxLocation + rotatedHalfExtents;
}


void UPlayer::ScreenToRay(float screenX, float screenY, const FMatrix& viewMatrix, const FMatrix& projectionMatrix, FVector& rayOrigin, FVector& rayDir)
{
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	FEngineLoop::graphicDevice.DeviceContext->RSGetViewports(&numViewports, &viewport);
	float screenWidth = viewport.Width;
	float screenHeight = viewport.Height;
	float x = (2.0f * screenX) / screenWidth - 1.0f;
	float y = 1.0f - (2.0f * screenY) / screenHeight;

	// �������� ����� ���
	FMatrix inverseProj = FMatrix::Inverse(projectionMatrix);

	// NDC���� �� �������� ��ȯ
	FVector4 nearPoint = inverseProj.TransformFVector4(FVector4(x, y, 0.0f, 1.0f));
	FVector4 farPoint = inverseProj.TransformFVector4(FVector4(x, y, 1.0f, 1.0f));

	// W�� 1�� ����ȭ
	nearPoint = nearPoint / nearPoint.a;
	farPoint = farPoint / farPoint.a;

	// �� ����� �ݿ��Ͽ� ���� �������� ��ȯ
	FMatrix inverseView = FMatrix::Inverse(viewMatrix);
	FVector nearWorld = inverseView.TransformPosition(FVector(nearPoint.x, nearPoint.y, nearPoint.z));
	FVector farWorld = inverseView.TransformPosition(FVector(farPoint.x, farPoint.y, farPoint.z));

	// ������ �������� ���� ���
	rayOrigin = nearWorld;
	rayDir = farWorld - nearWorld;
	rayDir = rayDir.Normalize();

	char message[256];
	sprintf_s(message, "Ray Origin: (%.2f, %.2f, %.2f)\nRay Direction: (%.2f, %.2f, %.2f)",
		rayOrigin.x, rayOrigin.y, rayOrigin.z, rayDir.x, rayDir.y, rayDir.z);
	//MessageBoxA(nullptr, message, "ScreenToRay Output", MB_OK);
}

bool UPlayer::RayIntersectsSphere(const FVector& rayOrigin, const FVector& rayDir, const FVector& sphereCenter, float sphereRadius)
{
	// ���̿� ��ü�� ���� ���� ���
	FVector oc;
	oc.x = rayOrigin.x - sphereCenter.x;
	oc.y = rayOrigin.y - sphereCenter.y;
	oc.z = rayOrigin.z - sphereCenter.z;

	float b = 2.0f * (rayDir.x * oc.x + rayDir.y * oc.y + rayDir.z * oc.z);
	float c = (oc.x * oc.x + oc.y * oc.y + oc.z * oc.z) - sphereRadius * sphereRadius;

	float discriminant = b * b - 4.0f * c;
	return discriminant > 0;
}
bool UPlayer::RayIntersectsAABB(const FVector& rayOrigin, const FVector& rayDir, const FVector& boxMin, const FVector& boxMax)
{
	float tMin = (boxMin.x - rayOrigin.x) / rayDir.x;
	float tMax = (boxMax.x - rayOrigin.x) / rayDir.x;

	if (tMin > tMax) std::swap(tMin, tMax);

	float tyMin = (boxMin.y - rayOrigin.y) / rayDir.y;
	float tyMax = (boxMax.y - rayOrigin.y) / rayDir.y;

	if (tyMin > tyMax) std::swap(tyMin, tyMax);

	if ((tMin > tyMax) || (tyMin > tMax))
		return false;

	if (tyMin > tMin)
		tMin = tyMin;
	if (tyMax < tMax)
		tMax = tyMax;

	float tzMin = (boxMin.z - rayOrigin.z) / rayDir.z;
	float tzMax = (boxMax.z - rayOrigin.z) / rayDir.z;

	if (tzMin > tzMax) std::swap(tzMin, tzMax);

	if ((tMin > tzMax) || (tzMin > tMax))
		return false;

	return true;
}

