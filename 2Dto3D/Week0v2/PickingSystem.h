#pragma once
#include "Define.h"
#include "windows.h"
#include "UWorld.h"
#include "UCameraComponent.h"
#include "EngineLoop.h"
#include <d3d10.h>
class UPickingSystem {
public:
	POINT MousePos;
	float leftBorder;
	float topBorder;
	float bottomBorder;
	float screenWidth;
	float screenHeight;

	FVector rayOrigin;
	FVector rayDir;

	void InitPickingSystem();
	void UpdateMousePos();
	FVector GetNDCPos();
	FVector GetViewPos(FVector vProj);
	void ScreenToRay();
	bool RayIntersectAABB(FVector& loc, FVector& boxMin, FVector& boxMax);

	void Input();
};