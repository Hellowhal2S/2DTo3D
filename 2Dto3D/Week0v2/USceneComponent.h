#pragma once
#include "UObject.h"
#include "VectorUtils.h"
#include "Renderer.h"
#include "PrimitiveDatas.h"
#include "Define.h"
//#include <d3d11.h>  // DirectX 헤더
//#include "Define.h"
//#include "UObject.h"
//#include "VectorUtils.h"
//#include "Renderer.h"
//#include "PrimitiveDatas.h"
//#include "UWorld.h"  // 가장 마지막에 포함
class UWorld;
//class UObject;
class USceneComponent : public UObject
{
public:
	USceneComponent(){};
	virtual ~USceneComponent(){};
	
	FVector RelativeLocation;
	FVector RelativeRotation;
	FVector RelativeScale3D;

	FVector GetUpVector();
	FVector GetForwardVector();
	FVector GetRightVector();
	/*virtual void Init(UWorld* _World) override {};
	virtual void Update(double deltaTime) override {};
	virtual void Release() override {};*/
	virtual void Render(URenderer& renderer, FMatrix MVP){};
};

class UPrimitiveComponent : public USceneComponent
{
public:
	uint32 objIndex;
	ID3D11Buffer* vertexBuffer;
	uint32 numvertices;
	/*virtual void Init(UWorld* _World) override {};
	virtual void Update(double deltaTime) override {};
	virtual void Release() override {};*/
	virtual void Render(URenderer& renderer, FMatrix MVP) {};
	//virtual void Render(URenderer& renderer, FMatrix MVP) {};
};

class UCubeComp : public UPrimitiveComponent
{
public:
	//UCubeComp();
	//~UCubeComp();
	//
	static FVector initPos;
	static FVector initRot;
	static FVector initScale;

	static void NewCube(UWorld* myWorld);
	/*void Init(UWorld* _World) override;
	void Update(double deltaTime) override;
	void Release() override;*/
	void Render(URenderer& renderer, FMatrix MVP) override;
};

class USphereComp : public UPrimitiveComponent
{
public:
	/*USphereComp();
	~USphereComp();*/

	static FVector initPos;
	static FVector initRot;
	static FVector initScale;

	static void NewSphere(UWorld* myWorld);
	/*void Init(UWorld* _World) override;
	void Update(double deltaTime) override;
	void Release() override;*/
	void Render(URenderer& renderer, FMatrix MVP) override;
};