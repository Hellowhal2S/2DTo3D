#include "USceneComponent.h"
#include "Define.h"

FVector USceneComponent::GetForwardVector()
{
	FVector Forward = FVector(0.f, 0.f, 1.0f);
	Forward = Utils::FVectorRotate(Forward, RelativeRotation);
	return Forward;
}

FVector USceneComponent::GetRightVector() {
	FVector Right = FVector(1.f, 0.f, 0.f);
	Right = Utils::FVectorRotate(Right, RelativeRotation);
	return Right;
}