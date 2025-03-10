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
	// TOPOLOGY�� ���θ���Ʈ�� �����Ѵ�.
	FEngineLoop::graphicDevice.DeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

	FMatrix Model = JungleMath::CreateModelMatrix(GetLocation(), GetRotation(), GetScale());

	FMatrix MVP = Model * GetEngine().View * GetEngine().Projection;
	if (this == GetWorld()->GetPickingObj()) {
		FEngineLoop::renderer.UpdateConstant(MVP, 1.0f);
	}
	else
		FEngineLoop::renderer.UpdateConstant(MVP, 0.0f);

	FEngineLoop::renderer.RenderPrimitive(FEngineLoop::resourceManager.vertexBufferGizmo, FEngineLoop::resourceManager.numVerticesGizmo);
	
	// TOPOLOGY�� ������� �������´�.
	FEngineLoop::graphicDevice.DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}
