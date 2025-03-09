#pragma once
#include <Windows.h>
#include "GraphicDevice.h"

class FRenderer;
class FResourceMgr
{
public:
	void Initialize(FRenderer* renderer);
	void Release(FRenderer* renderer);
public:
	ID3D11Buffer* vertexBufferSphere = nullptr;
	ID3D11Buffer* vertexBufferCube = nullptr;
	ID3D11Buffer* vertexBufferGizmo = nullptr;
	UINT numVerticesSphere;
	UINT numVerticesCube;
	UINT numVerticesGizmo;
};

