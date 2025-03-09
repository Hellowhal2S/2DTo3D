#include "World.h"
#include "CameraComponent.h"
#include "SphereComp.h"
#include "CubeComp.h"
#include <DirectXMath.h>
#include "JungleMath.h"
#include "GizmoComponent.h"
#include "ObjectFactory.h"
#include "Player.h"
#include "ArrowComp.h"
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
	Camera->SetLocation(FVector(0, 0, -10.f));
	GUObjectArray.push_back(Camera);
	camera = Camera;

	UObject* Sphere = FObjectFactory::ConstructObject<USphereComp>();
	GUObjectArray.push_back(Sphere);

	UObject* Cube = FObjectFactory::ConstructObject<UCubeComp>();
	Cube->SetLocation(FVector(10.f, 0.0f, 0.f));
	GUObjectArray.push_back(Cube);
	UObject* gizmo = FObjectFactory::ConstructObject<UGizmoComponent>();
	gizmo->SetScale(FVector(100000.0f, 100000.0f, 100000.0f));
	GUObjectArray.push_back(gizmo);

	UObject* localGizmo = FObjectFactory::ConstructObject<UArrowComp>();
	LocalGizmo[0] = localGizmo;
	GUObjectArray.push_back(localGizmo);
	localGizmo = FObjectFactory::ConstructObject<UArrowComp>();
	LocalGizmo[1] = localGizmo;
	localGizmo->SetRotation(FVector(0.0f, 90.0f, 0.0));
	GUObjectArray.push_back(localGizmo);
	localGizmo = FObjectFactory::ConstructObject<UArrowComp>();
	localGizmo->SetRotation(FVector(0.0f, 0.0f, -90.f));
	LocalGizmo[2] = localGizmo;
	GUObjectArray.push_back(localGizmo);

#pragma region MyRegion
	// 
	//for (int32 i = 0;i < OBJ_END;++i)
	//{
	//	m_pObjectList.push_back(TDoubleLinkedList<UObject*>());
	//}
	//UObject* player = FObjectFactory::ConstructObject<UPlayer>();
	//player->Initialize();
	//m_pObjectList[OBJ_PLAYER].push_back(player);

	//UObject* Camera = new UCameraComponent;
	//Camera->Initialize();
	//Camera->SetLocation(FVector(0, 0, -10.f));
	//m_pObjectList[OBJ_CAMERA].push_back(Camera);


	//UObject* Sphere = new USphereComp;
	//Sphere->Initialize();

	//m_pObjectList[OBJ_SPHERE].push_back(Sphere);
	//UObject* Cube = new UCubeComp;
	//Cube->Initialize();
	//Cube->SetLocation(FVector(10.f, 0.0f, 0.f));
	//m_pObjectList[OBJ_CUBE].push_back(Cube);

	//UObject* gizmo = new UGizmoComponent;
	//gizmo->Initialize();
	////gizmo->SetLocation(FVector(0.f, 0.0f, 0.f));

	//UObject* localGizmo = new UCubeComp;
	//localGizmo->Initialize();
	////localGizmo->SetLocation(FVector(1.f, 0.0f, 0.0f));
	//localGizmo->SetScale(FVector(2.0f, .1f, 0.1f));
	//LocalGizmo[0] = localGizmo;
	//localGizmo = new UCubeComp;
	//localGizmo->Initialize();
	////localGizmo->SetLocation(FVector(0.0f, 1.f, 0.0f));
	//localGizmo->SetScale(FVector(.1f, 2.0f, .1f));
	//LocalGizmo[1] = localGizmo;
	//localGizmo = new UCubeComp;
	//localGizmo->Initialize();
	////localGizmo->SetLocation(FVector(0.0f, 0.0f, 1.f));
	//localGizmo->SetScale(FVector(.1f, .1f, 2.0f));
	//LocalGizmo[2] = localGizmo;
	//gizmo->Initialize();

#pragma endregion

	

}

void UWorld::Tick(double deltaTime)
{
	Input();
	//for (int32 i = 0;i < m_pObjectList.size();i++)
	//{
	//	for (auto iter = m_pObjectList[i].begin();iter != m_pObjectList[i].end();++iter)
	//	{
	//		(*iter)->Update(deltaTime);
	//	}
	//}
	for (auto iter : GUObjectArray)
	{
		iter->Update(deltaTime);
	}
	if (pickingObj)
	{
		LocalGizmo[0]->SetLocation(pickingObj->GetLocation());
		LocalGizmo[0]->SetRotation(FVector(0.0f,0.0f,0.f) + pickingObj->GetRotation());
		LocalGizmo[1]->SetLocation(pickingObj->GetLocation());
		LocalGizmo[1]->SetRotation(FVector(0.0f, 90.0f, 0.f) + pickingObj->GetRotation());
		LocalGizmo[2]->SetLocation(pickingObj->GetLocation());
		LocalGizmo[2]->SetRotation(FVector(0.0f, 0.0f, -90.f) + pickingObj->GetRotation());
	}
}

void UWorld::Release()
{
	//for (int32 i = 0;i < m_pObjectList.size();i++)
	//{
	//	for (auto iter = m_pObjectList[i].begin();iter != m_pObjectList[i].end();++iter)
	//	{
	//		delete (*iter);
	//	}
	//	m_pObjectList[i].clear();
	//}
	//delete worldGizmo;
	for (auto iter : GUObjectArray)
	{
		delete iter;
	}
	GUObjectArray.clear();
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
		//m_pObjectList[OBJ_SPHERE].push_back(pObj);
		GUObjectArray.push_back(pObj);
		break;
	case OBJ_TRIANGLE:
		break;
	case OBJ_CUBE:
		pObj = FObjectFactory::ConstructObject<UCubeComp>();
		GUObjectArray.push_back(pObj);

		break;
	default:
		break;
	}
	pickingObj = pObj;
}

void UWorld::LoadData(SceneData& _Data)
{
	for (auto iter = GUObjectArray.begin(); iter != GUObjectArray.end();)
	{
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
