//#include "UObject.h"
#include "USceneComponent.h"
#include "Define.h"
#include "UWorld.h"


//USceneComponent::USceneComponent()
//{
//}
//
//USceneComponent::~USceneComponent()
//{
//}

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

// Sphere 초기값
FVector USphereComp::initPos = FVector(0, 0, 0);
FVector USphereComp::initRot = FVector(0, 0, 0);
FVector USphereComp::initScale = FVector(1, 1, 1);

void USphereComp::NewSphere(UWorld* myWorld) {
	
	USphereComp* newSphere = new USphereComp;
	newSphere->RelativeLocation = USphereComp::initPos;
	newSphere->RelativeRotation = USphereComp::initRot;
	newSphere->RelativeScale3D = USphereComp::initScale;
	newSphere->vertexBuffer = PrimitiveDatas::bufferList[OBJ_SPHERE];
	newSphere->numvertices = PrimitiveDatas::numVerticesList[OBJ_SPHERE];
	
	UObject::GUObjectArray.push_back(newSphere);
	//myWorld->GetObjectLists()[OBJ_SPHERE].push_back(newSphere);
}
void USphereComp::Render(URenderer& renderer, FMatrix MVP) {
	renderer.UpdateConstant(MVP);
	renderer.RenderPrimitive(PrimitiveDatas::bufferList[OBJECTS::OBJ_SPHERE], PrimitiveDatas::numVerticesList[OBJECTS::OBJ_SPHERE]);
}
//void USphereComp::Init(UWorld* _World) {
//
//}
//
//void USphereComp::Update(double deltaTime) {
//
//}
//
//void USphereComp::Release(){
//
//}

// cube 초기값
FVector UCubeComp::initPos = FVector(0, 0, 0);
FVector UCubeComp::initRot = FVector(0, 0, 0);
FVector UCubeComp::initScale = FVector(1, 1, 1);

void UCubeComp::Render(URenderer& renderer, FMatrix MVP){
	renderer.UpdateConstant(MVP);
	renderer.RenderPrimitive(PrimitiveDatas::bufferList[OBJECTS::OBJ_CUBE], PrimitiveDatas::numVerticesList[OBJECTS::OBJ_CUBE]);
}
void UCubeComp::NewCube(UWorld* myWorld)
{
	UCubeComp* newCube = new UCubeComp;
	newCube->RelativeLocation = UCubeComp::initPos;
	newCube->RelativeRotation = UCubeComp::initRot;
	newCube->RelativeScale3D = UCubeComp::initScale;

	UObject::GUObjectArray.push_back(newCube);
	UWorld::myWorld->GetObjectLists()[OBJ_CUBE].push_back(newCube);
}
//void UCubeComp::Init(UWorld* _World) {
//	
//}
//
//void UCubeComp::Update(double deltaTime) {
//
//}
//
//void UCubeComp::Release() {
//
//}

