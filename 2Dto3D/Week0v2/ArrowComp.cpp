#include "ArrowComp.h"
#include "JungleMath.h"

UArrowComp::UArrowComp() : UPrimitiveComponent("Arrow")
{
}

UArrowComp::~UArrowComp()
{
}

void UArrowComp::Initialize()
{
}

void UArrowComp::Update(double deltaTime)
{
}

void UArrowComp::Release()
{
}

void UArrowComp::Render()
{
	FMatrix Model = JungleMath::CreateModelMatrix(GetLocation(), GetRotation(), GetScale());

	// 최종 MVP 행렬
	FMatrix MVP = Model * GetEngine().View * GetEngine().Projection;
	if (this == GetWorld()->GetPickingObj()) {
		FEngineLoop::renderer.UpdateConstant(MVP, 1.0f);
	}
	else
		FEngineLoop::renderer.UpdateConstant(MVP, 0.0f);

	FEngineLoop::renderer.RenderPrimitive(FEngineLoop::resourceMgr.vertexBufferArrow, FEngineLoop::resourceMgr.numVerticesArrow);
}
