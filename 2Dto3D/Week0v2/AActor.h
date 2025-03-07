#pragma once
#include "UObject.h"
#include "VectorUtils.h"
class AActor : public UObject {
private:
	// Ʈ������ ������
	FVector location;
	FVector rotation;
	FVector scale;

#pragma region ���� ���� ����
	/* ���� ����
	// ���� ����
	UWorld* myUWorld;
	// �θ� ����
	AActor* pActor;
	// ���� ��Ʈ ������Ʈ
	USceneComponent* ROotComponent;
	// ������Ʈ?
	// BeginPlay
	// EndPlay
	// Destroty()
	*/
#pragma endregion
public:
	// �⺻ ������
	AActor();
	// �Ҹ���
	virtual ~AActor();
	

	// Ʈ������ ���� ����, ����
	FVector GetActorLocation() {
		return location;
	}
	void SetActorLocation(FVector newLoc) {
		location = newLoc;
	}

	FVector GetActorRotation() {
		return rotation;
	}
	void SetActorRotation(FVector newRot) {
		rotation = newRot;
	}

	FVector GetActorScale() {
		return scale;
	}
	void SetActorScale(FVector newScale) {
		scale = newScale;
	}

	// ���� ��ü ��ġ
	AActor* SpawnActor() {

	}

};