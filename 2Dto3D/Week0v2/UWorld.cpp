#include "UWorld.h"
#include "UCameraComponent.h"
#include "UObject.h"           
#include <d3d11.h>

UWorld::UWorld() {
}

UWorld::~UWorld() {
	ReleaseWorld();
}

void UWorld::InitWorld() {
	for (int i = 0;i < OBJ_END;++i)
	{
		ObjectLists.push_back(TDoubleLinkedList<UObject*>());
	}
	//UCameraComponent* mainCamera = UObject::NewObject();
	mainCamera = new UCameraComponent;
	
	//TEST_CODE
	UObject* sphere_obj = new USphereComp;
	USphereComp* sphere_comp = static_cast<USphereComp*>(sphere_obj);
	sphere_comp->RelativeLocation = FVector(0, 0, 0);
	sphere_comp->RelativeRotation = FVector(0, 0, 0);
	sphere_comp->RelativeScale3D = FVector(1, 1, 1);
	PrimitiveList.push_back(sphere_obj);

	mainCamera->Init(this);

	//테스트용 초기화Set
	mainCamera->SetEyePosition(FVector(0, 0, -10.0));

	ObjectLists[OBJ_CAMERA].push_back(mainCamera);
}

void UWorld::UpdateWorld(double deltaTime) {
	for (int i = 0; i < ObjectLists.size();i++) {
		for (auto iter = ObjectLists[i].begin(); iter != ObjectLists[i].end();++iter) {
			(*iter)->Update(deltaTime);
		}
	}
}

void UWorld::RenderWorld() {
	/*for (int i = 0; i < ObjectLists.size();i++) {
		for(auto iter = ObjectLists.begin(); iter!)
	}*/
}

void UWorld::ReleaseWorld() {

}

TArray<TDoubleLinkedList<UObject*>> UWorld::GetObjectLists() {
	return ObjectLists;
}

TArray<UObject*> UWorld::GetPrimitiveList() {
	return PrimitiveList;
}