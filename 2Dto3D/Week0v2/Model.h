#pragma once
#include "PrimitiveComponent.h"


class UCubeComp : public UPrimitiveComponent
{
public:
    UCubeComp();
     ~UCubeComp();
};

class USphereComp : public UPrimitiveComponent
{
public:
    USphereComp();
     ~USphereComp();
};

class UGizmoComp : public UPrimitiveComponent
{
public:
    UGizmoComp();
    ~UGizmoComp();
    void SetTransform(const FVector& Location, const FVector& Rotation, const FVector& Scale);
};
