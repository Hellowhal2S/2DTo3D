#pragma once

#include "Define.h"
#include <d3d11.h>
#include "ContainerDefs.h"
#include "Sphere.h"
#include "Cube.h"
#include "Renderer.h"
class PrimitiveDatas {
public:
	static TArray<ID3D11Buffer*> bufferList;
	static TArray<uint32> numVerticesList;
	static void InitPrimitiveDatas(URenderer& renderer);	
};