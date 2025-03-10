#include "World.h"

#include "ObjectFactory.h"

#include "Player.h"

#include "CameraComponent.h"
#include "SphereComponent.h"
#include "CubeComponent.h"
#include "GizmoComponent.h"
#include "ArrowComponent.h"

#include "JungleMath.h"

UWorld::UWorld()
{
}

UWorld::~UWorld()
{
	Release();
}

void UWorld::Initialize()
{
	UObject* player = FObjectFactory::ConstructObject<UPlayer>();
	GUObjectArray.push_back(player);

	UObject* Camera = FObjectFactory::ConstructObject<UCameraComponent>();
	Camera->SetLocation(FVector(-10.0f, 0.0f, 0.f));
	GUObjectArray.push_back(Camera);
	camera = Camera;

	//월드 좌표계 기즈모 생성
	UObject* gizmo = FObjectFactory::ConstructObject<UGizmoComponent>();
	gizmo->SetScale(FVector(100000.0f, 100000.0f, 100000.0f));
	GUObjectArray.push_back(gizmo);
	worldGizmo = gizmo;

	//UObject* Sphere = FObjectFactory::ConstructObject<USphereComp>();
	//GUObjectArray.push_back(Sphere);

	//UObject* Cube = FObjectFactory::ConstructObject<UCubeComp>();
	//Cube->SetLocation(FVector(10.f, 0.0f, 0.f));
	//GUObjectArray.push_back(Cube);

	//UObject* localGizmo = FObjectFactory::ConstructObject<UArrowComp>();
	//LocalGizmo[0] = localGizmo;
	//GUObjectArray.push_back(localGizmo);
	//localGizmo = FObjectFactory::ConstructObject<UArrowComp>();
	//LocalGizmo[1] = localGizmo;
	//localGizmo->SetRotation(FVector(0.0f, 90.0f, 0.0));
	//GUObjectArray.push_back(localGizmo);
	//localGizmo = FObjectFactory::ConstructObject<UArrowComp>();
	//localGizmo->SetRotation(FVector(0.0f, 0.0f, -90.f));
	//LocalGizmo[2] = localGizmo;
	//GUObjectArray.push_back(localGizmo);
}

void UWorld::Release()
{
	for (auto iter : GUObjectArray)
	{
		delete iter;
	}
	GUObjectArray.clear();
}

void UWorld::Tick(double deltaTime)
{
	Input();
	for (auto iter : GUObjectArray)
	{
		iter->Update(deltaTime);
	}
	//if (pickingObj)
	//{
	//	LocalGizmo[0]->SetLocation(pickingObj->GetLocation());
	//	LocalGizmo[0]->SetRotation(FVector(0.0f, 0.0f, 0.f) + pickingObj->GetRotation());
	//	LocalGizmo[1]->SetLocation(pickingObj->GetLocation());
	//	LocalGizmo[1]->SetRotation(FVector(0.0f, 90.0f, 0.f) + pickingObj->GetRotation());
	//	LocalGizmo[2]->SetLocation(pickingObj->GetLocation());
	//	LocalGizmo[2]->SetRotation(FVector(0.0f, 0.0f, -90.f) + pickingObj->GetRotation());
	//}
}

void UWorld::Render()
{
}

void UWorld::Input()
{
}

void UWorld::SpawnObject(OBJECTS _Obj)
{
	UObject* pObj = nullptr;
	switch (_Obj)
	{
	case OBJ_SPHERE:
		pObj = FObjectFactory::ConstructObject<USphereComp>();
		GUObjectArray.push_back(pObj);
		break;
	case OBJ_CUBE:
		pObj = FObjectFactory::ConstructObject<UCubeComp>();
		GUObjectArray.push_back(pObj);
		break;
	default:
		break;
	}
	//생성된 Obj에 Focus
	pickingObj = pObj;
}

void UWorld::LoadData(SceneData& _Data)
{
	for (auto iter = GUObjectArray.begin(); iter != GUObjectArray.end();)
	{
		// 물체(PrimitiveComponent)만 삭제
		UPrimitiveComponent* Primitive = dynamic_cast<UPrimitiveComponent*>(*iter);
		if (Primitive)
		{
			delete (*iter);
			iter = GUObjectArray.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	//SceneData의 Primitive들을 GUObjectArray에 추가
	for (auto iter : _Data.Primitives)
	{
		GUObjectArray.push_back(iter.second);
	}
}

SceneData UWorld::SaveData()
{
	SceneData Save;
	int32 Count = 0;

	for (auto iter : GUObjectArray)
	{
		UPrimitiveComponent* Primitive = dynamic_cast<UPrimitiveComponent*>(iter);
		if (Primitive)
		{
			Save.Primitives[Count] = iter;
			Count++;
		}
	}
	Save.Version = 1;
	Save.NextUUID = Count;

	return Save;
}

void UWorld::NewScene()
{
	Release();
	Initialize();
}
