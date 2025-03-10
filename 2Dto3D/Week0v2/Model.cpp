#include "Model.h"

UCubeComp::UCubeComp()
{
}
UCubeComp::~UCubeComp()
{
}
USphereComp::USphereComp()
{
}
USphereComp::~USphereComp()
{
}	
UGizmoComp::UGizmoComp()
{
}
UGizmoComp::~UGizmoComp()
{
}

void UGizmoComp::SetTransform(const FVector& Location, const FVector& Rotation, const FVector& Scale)
{
    RelativeLocation = Location;
    RelativeRotation = Rotation;
    RelativeScale3D = Scale;
}
