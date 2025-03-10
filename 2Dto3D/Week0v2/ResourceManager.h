#pragma once

#include <Windows.h>

#include "Renderer.h"

class FResourceManager
{
public:
	void Initialize(FRenderer* renderer);
	void Release(FRenderer* renderer);

public:
	ID3D11Buffer*	vertexBufferSphere = nullptr;
	ID3D11Buffer*	vertexBufferCube = nullptr;
	ID3D11Buffer*	vertexBufferGizmo = nullptr;
	ID3D11Buffer*	vertexBufferArrow = nullptr;

	UINT			numVerticesSphere;
	UINT			numVerticesCube;
	UINT			numVerticesGizmo;
	UINT			numVerticesArrow;
};

