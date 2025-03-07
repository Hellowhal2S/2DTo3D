#pragma once
class UObject {
public:
	// �⺻ ������
	UObject();
	// ��ü ���� �� ȣ��
	virtual ~UObject();

	uint32_t UUID;
	uint32_t Internalndex;

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
	UObject* NewObject();
	// ���� ���� ���� ���� ���� ����
	
	// ���� �����͵� NewObject
};

TArray<UObject*> GUObjectArray;