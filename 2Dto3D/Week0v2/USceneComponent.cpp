#include "USceneComponent.h"
#include "Define.h"

FVector USceneComponent::GetUpVector() {
	FVector Up = FVector(0.f, 1.0f, 0.f);
	Up = Utils::FVectorRotate(Up, RelativeRotation);
	return Up;
}
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