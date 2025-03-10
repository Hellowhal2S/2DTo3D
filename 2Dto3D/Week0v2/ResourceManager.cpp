#include "ResourceManager.h"

#include "Sphere.h"
#include "Cube.h"
#include "Gizmo.h"
#include "Arrow.h"

void FResourceManager::Initialize(FRenderer* renderer)
{
	vertexBufferSphere = renderer->CreateVertexBuffer(sphere_vertices, sizeof(sphere_vertices));
	numVerticesSphere = sizeof(sphere_vertices) / sizeof(FVertexSimple);

	vertexBufferCube = renderer->CreateVertexBuffer(cube_vertices, sizeof(cube_vertices));
	numVerticesCube = sizeof(cube_vertices) / sizeof(FVertexSimple);

	vertexBufferGizmo = renderer->CreateVertexBuffer(gizmoVertices, sizeof(gizmoVertices));
	numVerticesGizmo = sizeof(gizmoVertices) / sizeof(FVertexSimple);

	vertexBufferArrow = renderer->CreateVertexBuffer(arrow_vertices, sizeof(arrow_vertices));
	numVerticesArrow = sizeof(arrow_vertices) / sizeof(FVertexSimple);
}

void FResourceManager::Release(FRenderer* renderer)
{
	renderer->ReleaseVertexBuffer(vertexBufferSphere);
	renderer->ReleaseVertexBuffer(vertexBufferCube);
	renderer->ReleaseVertexBuffer(vertexBufferGizmo);
	renderer->ReleaseVertexBuffer(vertexBufferArrow);
}
