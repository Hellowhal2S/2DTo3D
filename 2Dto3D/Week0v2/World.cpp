#include "World.h"
#include "CameraComponent.h"
UWorld::UWorld()
{
}

UWorld::~UWorld()
{
}

void UWorld::Initialize()
{
	for (int i = 0;i < OBJ_END;++i)
	{
		m_pObjectList.push_back(TDoubleLinkedList<UObject*>());
	}
	UObject* Camera = new UCameraComponent;
	Camera->Initialize(this);

	m_pObjectList[OBJ_CAMERA].push_back(Camera);
}

void UWorld::Update(double deltaTime)
{
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
}

void UWorld::Render()
{
}
