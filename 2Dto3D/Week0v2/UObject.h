#pragma once
#include <cstdint>
#include <type_traits>
#include "ContainerDefs.h"

//MOD
class UWorld;
class UObject {
public:
	// 기본 생성자
	UObject();
	// 객체 삭제 시 호출
	virtual ~UObject();

	uint32_t UUID;
	uint32_t Internalndex;

	//template <typename T>
	//static UObject* NewObject();

	virtual void Init(UWorld* _World) {};
	virtual void Update(double deltaTime) {};
	virtual void Release() {};

	static TArray<UObject*> GUObjectArray;
#pragma region  추후 구현 함수들
	/*
	// 객체 이름 반환
	FString GetName() const;
	// 객체 특정 클래스 상속받았는지 확인
	bool ISA(const UClass* SomeClass) const;
	// 객체 특정 클래스 정보 반환
	virtual UClass* GetClass() const;
	// 부모 객체 반환
	UObject* GetOuter() const;

	// 객체 삭제 요청
	void MarkPendingKill();
	// 객체가 삭제되었느지 확인
	bool IsPendingKill() const;
	
protected:
	// 가비지 컬렉션 위한 데이터
	EObjectFlags objectFlags;

	// 객체의 부모(소유 관계)
	UObject* Outer;
	UObject* NewObject();
	*/
#pragma endregion
	// 도형 정점 정보 저장 파일 관리
	
	// 정점 데이터들 NewObject
	
};