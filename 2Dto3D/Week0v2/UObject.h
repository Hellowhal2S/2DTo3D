#pragma once
#include <cstdint>
#include <type_traits>
#include "ContainerDefs.h"

//MOD
class UWorld;
class UObject {
public:
	// �⺻ ������
	UObject();
	// ��ü ���� �� ȣ��
	virtual ~UObject();

	uint32_t UUID;
	uint32_t Internalndex;

	//template <typename T>
	//static UObject* NewObject();

	virtual void Init(UWorld* _World) {};
	virtual void Update(double deltaTime) {};
	virtual void Release() {};

	static TArray<UObject*> GUObjectArray;
#pragma region  ���� ���� �Լ���
	/*
	// ��ü �̸� ��ȯ
	FString GetName() const;
	// ��ü Ư�� Ŭ���� ��ӹ޾Ҵ��� Ȯ��
	bool ISA(const UClass* SomeClass) const;
	// ��ü Ư�� Ŭ���� ���� ��ȯ
	virtual UClass* GetClass() const;
	// �θ� ��ü ��ȯ
	UObject* GetOuter() const;

	// ��ü ���� ��û
	void MarkPendingKill();
	// ��ü�� �����Ǿ����� Ȯ��
	bool IsPendingKill() const;
	
protected:
	// ������ �÷��� ���� ������
	EObjectFlags objectFlags;

	// ��ü�� �θ�(���� ����)
	UObject* Outer;
	UObject* NewObject();
	*/
#pragma endregion
	// ���� ���� ���� ���� ���� ����
	
	// ���� �����͵� NewObject
	
};