#pragma once
#include <cstdint>

class MemoryManager {
private:
	// �Ҵ�� ��ü ����Ʈ ��
	static uint32_t TotalAllocationBytes;
	// �Ҵ�� �޸� ����� ����
	static uint32_t TotalAllocationCount;

public:
	static uint32_t GetTotalAllocationBytes() {
		return TotalAllocationBytes;
	}
	static void SetTotalAllocationBytes(uint32_t bytes) {
		/* FIXME : ���� ���� ���� �߰� */
		TotalAllocationBytes += bytes;
	}
	static uint32_t GetTotalAllocationCount() {
		return TotalAllocationCount;
	}
	static void SetTotalAllocationCount(uint32_t count) {
		/* FIXME : ���� ���� ���� �߰� */
		TotalAllocationCount += count;
	}

};