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
					//FVector BoxMin, BoxMax;
					//float MaxScale = max(max(pObj->GetScale().x, pObj->GetScale().y), pObj->GetScale().z);
					//if (RayIntersectsSphere(rayOrigin, rayDir, pObj->GetLocation(), MaxScale))
					//{
					//	if (!Possible)
					//		Possible = pObj;
					//	else if (Possible->GetLocation().Distance(rayOrigin) > (pObj->GetLocation().Distance(rayOrigin)))
					//		Possible = pObj;
					//}
					float minDistance = 10000000.0f;
					float Distance = 0.0f;
					if (RayIntersectsObject(rayOrigin, rayDir, pObj, Distance))
					{
			
						if (minDistance > Distance) {
							Possible = pObj;
							minDistance = Distance;
						}
			/*			if (!Possible)
							Possible = pObj;
						else if (Possible->GetLocation().Distance(rayOrigin) > (pObj->GetLocation().Distance(rayOrigin)))
							Possible = pObj;*/
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
					float minDistance = 1000000.0f;
					float Distance = 0.0f;
					if (RayIntersectsObject(rayOrigin, rayDir, GetWorld()->LocalGizmo[i], Distance))
					{
						if (minDistance > Distance)
						{
							GetWorld()->SetPickingGizmo(GetWorld()->LocalGizmo[i]);
							UE_LOG(LogLevel::Display, "%d", Arrow->GetDir());
							minDistance = Distance;
						}
					}
				}
			}	
		}
		else
		{
			// 마우스 이동량 계산
			if (GetWorld()->GetPickingObj() && GetWorld()->GetPickingGizmo()) {
				POINT currentMousePos;
				GetCursorPos(&currentMousePos);
				UObject* pObj = GetWorld()->GetPickingObj();
				// 마우스 이동 차이 계산
				int32 deltaX = currentMousePos.x - m_LastMousePos.x;
				int32 deltaY = currentMousePos.y - m_LastMousePos.y;
				UArrowComp* Arrow = static_cast<UArrowComp*>(GetWorld()->GetPickingGizmo());
				switch (Arrow->GetDir())
				{
				case AD_X:
					if (GetWorld()->GetCamera()->GetForwardVector().z >= 0)
						pObj->AddLocation(pObj->GetRightVector() * deltaX * 0.01f);
					else
						pObj->AddLocation(pObj->GetRightVector()* deltaX * -0.01f);
					break;
				case AD_Y:
					pObj->AddLocation((pObj->GetUpVector() * deltaY * 0.01f) * -1 );
					break;
				case AD_Z:
					if(GetWorld()->GetCamera()->GetForwardVector().x <= 0)
						pObj->AddLocation(pObj->GetForwardVector() * deltaX*0.01f);
					else
						pObj->AddLocation(pObj->GetForwardVector()* deltaX * -0.01f);
					break;
				default:
					break;
				}

				// 새로운 마우스 위치 저장
				m_LastMousePos = currentMousePos;
			}
		}
	}
	else
	{
		if (bLeftMouseDown) {
				UE_LOG(LogLevel::Warning, "Up");
				bLeftMouseDown = false; // 마우스 오른쪽 버튼을 떼면 상태 초기화
				GetWorld()->SetPickingGizmo(nullptr);
			
		}
	}
}


bool UPlayer::RayIntersectsBox(const FVector& rayOrigin, const FVector& rayDir, const FVector& boxMin, const FVector& boxMax, const FVector& boxRotation)
{
	// 1. 회전 행렬 생성 (FVector로 주어진 회전값을 roll, pitch, yaw로 해석하여 회전 행렬을 생성)
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(boxRotation.x, boxRotation.y, boxRotation.z);

	// 2. 레이의 원점과 방향을 로컬 좌표계로 변환
	XMVECTOR rayOriginVec = XMVectorSet(rayOrigin.x, rayOrigin.y, rayOrigin.z, 1.0f);
	XMVECTOR rayDirVec = XMVectorSet(rayDir.x, rayDir.y, rayDir.z, 0.0f);

	// 회전 행렬을 사용하여 레이를 로컬 좌표계로 변환
	XMMATRIX inverseRotationMatrix = XMMatrixInverse(nullptr, rotationMatrix);
	rayOriginVec = XMVector3TransformCoord(rayOriginVec, inverseRotationMatrix);
	rayDirVec = XMVector3TransformNormal(rayDirVec, inverseRotationMatrix);

	// 3. 로컬 좌표계에서 레이의 원점과 방향을 FVector로 변환
	FVector localRayOrigin(XMVectorGetX(rayOriginVec), XMVectorGetY(rayOriginVec), XMVectorGetZ(rayOriginVec));
	FVector localRayDir(XMVectorGetX(rayDirVec), XMVectorGetY(rayDirVec), XMVectorGetZ(rayDirVec));

	// 4. 박스의 교차 여부를 계산 (AABB 교차 검사)
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

	// 5. 레이가 직육면체와 교차하면 true 반환
	return true;
}


void UPlayer::GetBoxMinMax(const FVector& boxLocation, const FVector& boxScale, const FVector& boxRotation, FVector& boxMin, FVector& boxMax)
{
	// 1. 박스 크기의 반쪽 (half-extents) 계산
	FVector halfExtents = boxScale * 0.5f;

	// 2. 회전 행렬을 생성 (FVector를 회전 행렬로 변환)
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(boxRotation.x, boxRotation.y, boxRotation.z);

	// 3. 박스의 반쪽 크기를 회전시킴
	XMVECTOR halfExtentsVec = XMVectorSet(halfExtents.x, halfExtents.y, halfExtents.z, 0.0f);
	XMVECTOR rotatedHalfExtentsVec = XMVector3TransformNormal(halfExtentsVec, rotationMatrix);

	// 4. 회전된 반쪽 크기 값을 FVector로 변환
	FVector rotatedHalfExtents(
		XMVectorGetX(rotatedHalfExtentsVec),
		XMVectorGetY(rotatedHalfExtentsVec),
		XMVectorGetZ(rotatedHalfExtentsVec)
	);

	// 5. Box의 min, max 값 계산
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

	// 프로젝션 역행렬 계산
	FMatrix inverseProj = FMatrix::Inverse(projectionMatrix);

	// NDC에서 뷰 공간으로 변환
	FVector4 nearPoint = inverseProj.TransformFVector4(FVector4(x, y, 0.0f, 1.0f));
	FVector4 farPoint = inverseProj.TransformFVector4(FVector4(x, y, 1.0f, 1.0f));

	// W를 1로 정규화
	nearPoint = nearPoint / nearPoint.a;
	farPoint = farPoint / farPoint.a;

	// 뷰 행렬을 반영하여 월드 공간으로 변환
	FMatrix inverseView = FMatrix::Inverse(viewMatrix);
	FVector nearWorld = inverseView.TransformPosition(FVector(nearPoint.x, nearPoint.y, nearPoint.z));
	FVector farWorld = inverseView.TransformPosition(FVector(farPoint.x, farPoint.y, farPoint.z));

	// 레이의 시작점과 방향 계산
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
	// 레이와 구체의 교차 여부 계산
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

//OBB로 수정
bool UPlayer::RayIntersectsObject(const FVector& rayOrigin, const FVector& rayDir, UObject* obj, float& hitDistance)
{
	// 오브젝트의 월드 변환 행렬 생성 (위치, 회전 적용)
	FMatrix rotationMatrix = FMatrix::CreateRotation(
		obj->GetRotation().x,
		obj->GetRotation().y,
		obj->GetRotation().z
	);

	FMatrix translationMatrix = FMatrix::CreateTranslationMatrix(obj->GetLocation());

	// 최종 변환 행렬
	FMatrix worldMatrix = rotationMatrix * translationMatrix;

	// OBB의 로컬 X, Y, Z 축 가져오기
	FVector axisX(worldMatrix.M[0][0], worldMatrix.M[0][1], worldMatrix.M[0][2]);
	FVector axisY(worldMatrix.M[1][0], worldMatrix.M[1][1], worldMatrix.M[1][2]);
	FVector axisZ(worldMatrix.M[2][0], worldMatrix.M[2][1], worldMatrix.M[2][2]);

	// 기즈모인지 확인
	//bool isGizmo = (dynamic_cast<UGizmoComp*>(obj) != nullptr);

	// OBB의 반 크기 적용 (기즈모의 경우 Y,Z 스케일을 0.2로 조정)
	FVector halfSize = obj->GetScale();
	//if (isGizmo)
	//{
	//	halfSize.y = 0.2f;
	//	halfSize.z = 0.2f;
	//}

	FVector p = obj->GetLocation() - rayOrigin; // 레이 원점에서 OBB 중심까지의 벡터
	FVector d = rayDir.Normalize(); // 레이 방향 정규화

	float tMin = -FLT_MAX, tMax = FLT_MAX;

	// 각 축(X, Y, Z)에 대해 레이와의 충돌 검사 수행
	FVector axes[3] = { axisX, axisY, axisZ };
	float halfSizes[3] = { halfSize.x, halfSize.y, halfSize.z };

	for (int i = 0; i < 3; i++)
	{
		float e = axes[i].Dot(p);  // 원점에서 타겟까지의 거리를 해당 축으로 투영
		float f = axes[i].Dot(d);  // 레이를 투영

		if (fabs(f) > 1e-6)  // 방향 벡터가 0이면 큰일남
		{
			float t1 = (e - halfSizes[i]) / f;
			float t2 = (e + halfSizes[i]) / f;

			if (t1 > t2) std::swap(t1, t2); // t1이 항상 작은 값이 되도록 설정

			tMin = max(tMin, t1);  // 범위를 계속 줄여나감
			tMax = min(tMax, t2);

			if (tMin > tMax) return false; // 충돌 없음
		}
		else if (-e - halfSizes[i] > 0 || -e + halfSizes[i] < 0)
		{
			return false; // 레이가 OBB 내부에 없고, 지나가지도 않음
		}
	}

	hitDistance = tMin;
	return true;
}