#pragma once
#include "SceneComponent.h"
#include "Renderer.h"
class UPrimitiveComponent : public USceneComponent {
public:
    UPrimitiveComponent();
    virtual ~UPrimitiveComponent();

protected:
    ID3D11Buffer* vertexBuffer = nullptr;
    UINT numVertices = 0;
};


