#include "World.h"
#include "CameraComponent.h"
#include "SphereComp.h"
#include "CubeComp.h"
#include <DirectXMath.h>
#include "JungleMath.h"
#include "GizmoComponent.h"
UWorld::UWorld()
{
}

UWorld::~UWorld()
{
	Release();
}

void UWorld::Initialize()
{
	for (int i = 0;i < OBJ_END;++i)
	{
		m_pObjectList.push_back(TDoubleLinkedList<UObject*>());
	}
	UObject* Camera = new UCameraComponent;
	Camera->Initialize(this);
	Camera->SetLocation(FVector(0, 0, -10.f));

	m_pObjectList[OBJ_CAMERA].push_back(Camera);

	UObject* Sphere = new USphereComp;
	Sphere->Initialize(this);

	m_pObjectList[OBJ_SPHERE].push_back(Sphere);
	UObject* Cube = new UCubeComp;
	Cube->Initialize(this);
	Cube->SetLocation(FVector(10.f, 0.0f, 0.f));
	m_pObjectList[OBJ_CUBE].push_back(Cube);

	UObject* gizmo = new UGizmoComponent;
	gizmo->Initialize(this);
	//gizmo->SetLocation(FVector(0.f, 0.0f, 0.f));

	UObject* localGizmo = new UCubeComp;
	localGizmo->Initialize(this);
	localGizmo->SetLocation(FVector(1.f, 0.0f, 0.0f));
	localGizmo->SetScale(FVector(2.0f, .1f, 0.1f));
	LocalGizmo[0] = localGizmo;
	localGizmo = new UCubeComp;
	localGizmo->Initialize(this);
	localGizmo->SetLocation(FVector(0.0f, 1.f, 0.0f));
	localGizmo->SetScale(FVector(.1f, 2.0f, .1f));
	LocalGizmo[1] = localGizmo;
	localGizmo = new UCubeComp;
	localGizmo->Initialize(this);
	localGizmo->SetLocation(FVector(0.0f, 0.0f, 1.f));
	localGizmo->SetScale(FVector(.1f, .1f, 2.0f));
	LocalGizmo[2] = localGizmo;
	gizmo->Initialize(this);
}

void UWorld::Update(double deltaTime)
{
	Input();
	for (int i = 0;i < m_pObjectList.size();i++)
	{
		for (auto iter = m_pObjectList[i].begin();iter != m_pObjectList[i].end();++iter)
		{
			(*iter)->Update(deltaTime);
		}
	}
}

void UWorld::Release()
{
	for (int i = 0;i < m_pObjectList.size();i++)
	{
		for (auto iter = m_pObjectList[i].begin();iter != m_pObjectList[i].end();++iter)
		{
			delete (*iter);
		}
		m_pObjectList[i].clear();
	}
	delete worldGizmo;
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
		pObj = new USphereComp;
		pObj->Initialize(this);
		m_pObjectList[OBJ_SPHERE].push_back(pObj);
		break;
	case OBJ_TRIANGLE:
		break;
	case OBJ_CUBE:
		pObj = new UCubeComp;
		pObj->Initialize(this);
		m_pObjectList[OBJ_CUBE].push_back(pObj);
		break;
	default:
		break;
	}
	pickingObj = pObj;
}
