#pragma once
#include "UObject.h"
#include "VectorUtils.h"
class AActor : public UObject {
private:
	// 트랜스폼 변수들
	FVector location;
	FVector rotation;
	FVector scale;

#pragma region 추후 구현 사항
	/* 추후 구현
	// 속한 월드
	UWorld* myUWorld;
	// 부모 액터
	AActor* pActor;
	// 액터 루트 컴포넌트
	USceneComponent* ROotComponent;
	// 컴포넌트?
	// BeginPlay
	// EndPlay
	// Destroty()
	*/
#pragma endregion
public:
	// 기본 생성자
	AActor();
	// 소멸자
	virtual ~AActor();
	

	// 트랜스폼 관련 게터, 세터
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

	// 씬에 객체 배치
	AActor* SpawnActor() {

	}

};