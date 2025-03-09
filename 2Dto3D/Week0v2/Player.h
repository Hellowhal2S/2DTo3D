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
	bool RayIntersectsAABB(const FVector& rayOrigin, const FVector& rayDir, const FVector& boxMin, const FVector& boxMax);
	bool RayIntersectsScaledSphere(const FVector& rayOrigin, const FVector& rayDir, const FVector& sphereCenter, const FVector& scale);


};

