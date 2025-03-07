#include "SceneComponent.h"
#include "World.h"

USceneComponent::USceneComponent()
{
}

USceneComponent::~USceneComponent()
{
}
void USceneComponent::Initialize(UWorld* _World)
{
	Super::Initialize(_World);
}

void USceneComponent::Update(double deltaTime)
{
}


void USceneComponent::Release()
{
}
