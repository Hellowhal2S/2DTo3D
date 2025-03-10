#pragma once
#include "Object.h"
class UPlayer : public UObject
{
public:
	UPlayer();
	virtual				~UPlayer();

	virtual void		Initialize();
	virtual void		Update(double deltaTime);
	virtual void		Release();

	void				Input();

private:
	void ScreenToRay(float screenX, float screenY, const FMatrix& viewMatrix, const FMatrix& projectionMatrix,
		FVector& rayOrigin, FVector& rayDir);
	bool RayIntersectsSphere(const FVector& rayOrigin, const FVector& rayDir,
		const FVector& sphereCenter, float sphereRadius);
	bool RayIntersectsObject(const FVector& rayOrigin, const FVector& rayDir, UObject* obj, float& hitDistance);

	bool bLeftMouseDown = false;
	bool bMouseClicked = false;
	POINT m_LastMousePos;
};

