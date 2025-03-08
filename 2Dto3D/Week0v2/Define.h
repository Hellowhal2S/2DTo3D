#pragma once
#include "VectorUtils.h"
#include "MatrixUtils.h"
enum OBJECTS
{
	OBJ_SPHERE,
	OBJ_CUBE,
	OBJ_TRIANGLE,
	OBJ_CAMERA,
	OBJ_END
};

class Utils{
	static FVector4 ConvertV3ToV4(FVector vec3);
};