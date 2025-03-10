#include "GizmoComponent.h"

UGizmoComponent::UGizmoComponent()
{
}

UGizmoComponent::~UGizmoComponent()
{
}

void UGizmoComponent::Initialize()
{
	Super::Initialize();
}

void UGizmoComponent::Update(double deltaTime)
{
}

void UGizmoComponent::Release()
{
}

void UGizmoComponent::Render()
{
	// TOPOLOGY를 라인리스트로 변경한다.
	FEngineLoop::graphicDevice.DeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

	FMatrix Model = JungleMath::CreateModelMatrix(GetLocation(), GetRotation(), GetScale());

	FMatrix MVP = Model * GetEngine().View * GetEngine().Projection;
	if (this == GetWorld()->GetPickingObj()) {
		FEngineLoop::renderer.UpdateConstant(MVP, 1.0f);
	}
	else
		FEngineLoop::renderer.UpdateConstant(MVP, 0.0f);

	FEngineLoop::renderer.RenderPrimitive(FEngineLoop::resourceManager.vertexBufferGizmo, FEngineLoop::resourceManager.numVerticesGizmo);
	
	// TOPOLOGY를 원래대로 돌려놓는다.
	FEngineLoop::graphicDevice.DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}
