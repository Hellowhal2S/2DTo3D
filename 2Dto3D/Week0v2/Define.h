#pragma once
#include "VectorUtils.h"
#include "MatrixUtils.h"
#include <DirectXMath.h>
using namespace DirectX;

enum OBJECTS
{
	OBJ_SPHERE,
	OBJ_CUBE,
	OBJ_TRIANGLE,
	OBJ_CAMERA,
	OBJ_END
};

class Utils{
public:
	static FVector4 ConvertV3ToV4(FVector vec3);
	static FVector FVectorRotate(FVector& origin, const FVector& rotation);
};