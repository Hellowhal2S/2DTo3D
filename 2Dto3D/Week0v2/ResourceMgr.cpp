#include "ResourceMgr.h"
#include "Sphere.h"
#include "Cube.h"
#include "Gizmo.h"
#include "Renderer.h"
#include "Define.h"
void FResourceMgr::Initialize(FRenderer* renderer)
{
	numVerticesSphere = sizeof(sphere_vertices) / sizeof(FVertexSimple);
	vertexBufferSphere = renderer->CreateVertexBuffer(sphere_vertices, sizeof(sphere_vertices));

	numVerticesCube = sizeof(cube_vertices) / sizeof(FVertexSimple);
	vertexBufferCube = renderer->CreateVertexBuffer(cube_vertices, sizeof(cube_vertices));


	numVerticesGizmo = sizeof(gizmoVertices) / sizeof(FVertexSimple);
	vertexBufferGizmo = renderer->CreateVertexBuffer(gizmoVertices, sizeof(gizmoVertices));
}

void FResourceMgr::Release(FRenderer* renderer)
{
	renderer->ReleaseVertexBuffer(vertexBufferSphere);
	renderer->ReleaseVertexBuffer(vertexBufferCube);
	renderer->ReleaseVertexBuffer(vertexBufferGizmo);
}
