#pragma once
#include "PrimitiveComponent.h"
#include "Cube.h"
#include "Sphere.h"

class UCubeComp : public UPrimitiveComponent
{
public:
    UCubeComp();
    virtual ~UCubeComp();

    virtual void Render() override;
};

class USphereComp : public UPrimitiveComponent
{
public:
    USphereComp();
    virtual ~USphereComp();

    virtual void Render() override;
};
