#include "UWorld.h"
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
	UObject* mainCamera = new UCameraComponent;
	//UCameraComponent* mainCamera = new UObject;
	//FIXME : 추후 전체 


	mainCamera->Init(this);

	//테스트용 초기화Set
	static_cast<UCameraComponent*>(mainCamera)->SetEyePosition(FVector(0, 0, -10.f));

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