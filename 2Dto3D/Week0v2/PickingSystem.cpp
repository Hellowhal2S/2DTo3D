#include "PickingSystem.h"


void UPickingSystem::InitPickingSystem()
{
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	DeviceManager::DeviceContext->RSGetViewports(&numViewports, &viewport);
	leftBorder = 0;
	topBorder = 0;
	screenWidth = viewport.Width;
	screenHeight = viewport.Height;
	//IDXGISwapChain* pSwapChain; // 스왑 체인 객체

	//DXGI_SWAP_CHAIN_DESC swapChainDesc;
	//pSwapChain->GetDesc(&swapChainDesc);
	//leftBorder = 0;
	//topBorder = 0;
	//screenWidth = swapChainDesc.BufferDesc.Width;		// = rightBorder
	//screenHeight = swapChainDesc.BufferDesc.Height;		// = bottomBorder
}
void UPickingSystem::UpdateMousePos()
{
	GetCursorPos(&MousePos);
	ScreenToClient(EngineLoop::hWnd, &MousePos);
}

FVector UPickingSystem::GetNDCPos() {
	FVector vProj;
	vProj.x = (((2.0f * MousePos.x - 2.0f * leftBorder) / screenWidth) - 1);
	vProj.y = -(((2.0f * MousePos.y - 2.0f * topBorder) / bottomBorder) - 1);
	vProj.z = 1.0f;
	return vProj;
}

FVector UPickingSystem::GetViewPos(FVector vProj)
{
	FVector vView;
	FMatrix pMat = UWorld::myWorld->mainCamera->GetProjectionMatrix();
	vView.x = vProj.x / pMat.M[1][1];
	vView.y = vProj.y / pMat.M[2][2];
	vView.z = vProj.z;
	return vView;
}

void UPickingSystem::ScreenToRay()
{
	FVector vView;
	vView = GetNDCPos();
	vView = GetViewPos(vView);

	FMatrix matInverse;
	matInverse = UWorld::myWorld->mainCamera->GetViewMatrix().Inverse();

	rayDir = FVector(vView.x, vView.y, 1.0f);
	rayOrigin = FVector(0, 0, 0);

	//ray 방향 변환
	rayDir.x = vView.x * matInverse.M[0][0] + vView.y * matInverse.M[1][0] + vView.z * matInverse.M[2][0];
	rayDir.y = vView.x * matInverse.M[0][1] + vView.y * matInverse.M[1][1] + vView.z * matInverse.M[2][1];
	rayDir.z = vView.x * matInverse.M[0][2] + vView.y * matInverse.M[1][2] + vView.z * matInverse.M[2][2];

	// ray 위치 변환
	rayOrigin.x = matInverse.M[3][0];
	rayOrigin.y = matInverse.M[3][1];
	rayOrigin.z = matInverse.M[3][2];

	// 방향 벡터 정규화 : 변환 후 방향 벡터의 크기를 1로 맞추어 픽킹 레이의 방향이 제대로 유지되도록.
	rayDir.Normalize();

}

bool UPickingSystem::RayIntersectAABB(FVector& loc, FVector& boxMin, FVector& boxMax) {
	float tMin = (boxMin.x - rayOrigin.x) / rayDir.x;
	float tMax = (boxMax.x - rayOrigin.x) / rayDir.x;

	if (tMin > tMax) std::swap(tMin, tMax);

	float tMiny = (boxMin.y - rayOrigin.y) / rayDir.y;
	float tMaxy = (boxMax.y - rayOrigin.y) / rayDir.y;

	if (tMiny > tMaxy) std::swap(tMiny, tMaxy);

	if ((tMin > tMaxy) || (tMiny > tMax)){
		return false;
	}

	if (tMiny > tMin)
		tMin = tMiny;
	if (tMaxy < tMax)
		tMax = tMaxy;

	float tMinz = (boxMin.z - rayOrigin.z) / rayDir.z;
	float tMaxz = (boxMax.z - rayOrigin.z) / rayDir.z;

	if (tMinz > tMaxz) std::swap(tMinz, tMaxz);

	if ((tMin > tMaxz) || (tMinz > tMax))
		return false;
	return true;
}
void UPickingSystem::Input() {
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
		UpdateMousePos();
		ScreenToRay();
		USceneComponent* Possible = nullptr;
		for (auto iter : UObject::GUObjectArray) {
			USceneComponent* comp = dynamic_cast<USceneComponent*>(iter);
			FVector boxMin = comp->RelativeLocation - comp->RelativeScale3D * 0.5f;
			FVector boxMax = comp->RelativeLocation + comp->RelativeScale3D * 0.5f;
			if (RayIntersectAABB(comp->RelativeLocation, boxMin, boxMax)) {
				// 픽킹 처리
				if (!Possible) {
					Possible = comp;
				}
				else if (Possible->RelativeLocation.Distance(rayOrigin) > comp->RelativeLocation.Distance(rayOrigin)) {
					Possible = comp;
				}
			}
		}
		if (Possible) {
			//UWorld::pickingObject = Possible;
			//UWorld::myWorld->SetPickingObject()
			//UWorld::myWorld -> SetPickingObject(Possible);
		}


	}
}