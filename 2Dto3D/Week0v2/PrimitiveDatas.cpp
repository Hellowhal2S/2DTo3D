#include "PrimitiveDatas.h"

TArray<ID3D11Buffer*> PrimitiveDatas::bufferList;
TArray<uint32> PrimitiveDatas::numVerticesList;

void PrimitiveDatas::InitPrimitiveDatas(URenderer& renderer) {
	TArray<FVertexSimple*> verticesList = { sphere_vertices, cube_vertices };
	TArray<const size_t*> sizeList = {&sphere_vertices_size, &cube_vertices_size };
	// RULE : primitive 추가 시 인덱스 범위 변경.
	for (int i = OBJECTS::OBJ_SPHERE; i != OBJECTS::OBJ_TRIANGLE; i++) {
		ID3D11Buffer* buffer =  renderer.CreateVertexBuffer(verticesList[i], *sizeList[i]);
		bufferList.push_back(buffer);
		numVerticesList.push_back(*sizeList[i] / sizeof(FVertexSimple));
	}
}
