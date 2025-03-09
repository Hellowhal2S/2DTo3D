#include "UPlayer.h"
#include <Windows.h>
#include "GraphicDevice.h"
#include "World.h"
#include "Define.h"
extern HWND hWnd;
extern UGraphicsDevice graphicDevice;
extern FMatrix View;
extern FMatrix Projection;
UPlayer::UPlayer()
{
}

UPlayer::~UPlayer()
{
}

void UPlayer::Initialize(UWorld* _World)
{
	Super::Initialize(_World);
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
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		POINT mousePos;
		GetCursorPos(&mousePos);
		ScreenToClient(hWnd, &mousePos);

		FVector rayOrigin;
		FVector rayDir;
		ScreenToRay(mousePos.x, mousePos.y, View, Projection, rayOrigin, rayDir);
		UObject* Possible = nullptr;

		for (auto iter = GetWorld()->GetSphreList().begin(); iter != GetWorld()->GetSphreList().end();++iter)
		{
			if (RayIntersectsSphere(rayOrigin, rayDir, (*iter)->GetLocation(), (*iter)->GetScale().x))
			{
				if (!Possible)
					Possible = (*iter);
				else if (Possible->GetLocation().Distance(rayOrigin) > ((*iter)->GetLocation().Distance(rayOrigin)))
					Possible = (*iter);
			}
		}

		for (auto iter = GetWorld()->GetCubeList().begin(); iter != GetWorld()->GetCubeList().end();++iter)
		{
			if (RayIntersectsSphere(rayOrigin, rayDir, (*iter)->GetLocation(), (*iter)->GetScale().x))
			{
				if (!Possible)
					Possible = (*iter);
				else if (Possible->GetLocation().Distance(rayOrigin) > ((*iter)->GetLocation().Distance(rayOrigin)))
					Possible = (*iter);
			}
		}
		if (Possible)
			GetWorld()->SetPickingObj(Possible);


		for (int i =0;i<3;++i)
		{
			float Scale =0.0f;
			if (i == 0)
				Scale = GetWorld()->LocalGizmo[0]->GetScale().x;
			else if (i == 1)
				Scale = GetWorld()->LocalGizmo[1]->GetScale().y;
			else if (i == 2)
				Scale = GetWorld()->LocalGizmo[2]->GetScale().z;
			if (RayIntersectsSphere(rayOrigin, rayDir, GetWorld()->LocalGizmo[i]->GetLocation(), 1.0f))
			{
				GetWorld()->SetPickingGizmo(GetWorld()->LocalGizmo[i]);
			}
			else
			{
				//char message[256];
				//sprintf_s(message, "Ray Origin: (%.2f, %.2f, %.2f)\nRay Direction: (%.2f, %.2f, %.2f)",
				//	rayOrigin.x, rayOrigin.y, rayOrigin.z, rayDir.x, rayDir.y, rayDir.z);
				//MessageBoxA(nullptr, message, "No", MB_OK);
			}
		}
	}
}

void UPlayer::ScreenToRay(float screenX, float screenY, const FMatrix& viewMatrix, const FMatrix& projectionMatrix, FVector& rayOrigin, FVector& rayDir)
{
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	graphicDevice.DeviceContext->RSGetViewports(&numViewports, &viewport);
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

//bool UPlayer::RayIntersectsScaledSphere(const FVector& rayOrigin, const FVector& rayDir, const FVector& sphereCenter, const FVector& scale)
//{
//	// 1. 스케일을 반전하여 '구' 형태로 변환
//	FVector invScale(1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z);
//
//	// 2. 레이의 원점과 방향을 변환된 공간으로 이동
//	FVector newOrigin = (rayOrigin - sphereCenter) * invScale;
//	FVector newDir = rayDir * invScale;
//
//	// 3. 정규화
//	float length = sqrt(newDir.x * newDir.x + newDir.y * newDir.y + newDir.z * newDir.z);
//	newDir.x /= length;
//	newDir.y /= length;
//	newDir.z /= length;
//
//	// 4. 기존 Sphere 충돌 판정 알고리즘 사용 (반지름 = 1)
//	float b = 2.0f * (newDir.x * newOrigin.x + newDir.y * newOrigin.y + newDir.z * newOrigin.z);
//	float c = (newOrigin.x * newOrigin.x + newOrigin.y * newOrigin.y + newOrigin.z * newOrigin.z) - 1.0f;
//	float discriminant = b * b - 4.0f * c;
//
//	return discriminant > 0;
//}