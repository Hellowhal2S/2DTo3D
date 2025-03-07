#pragma once
#include <cstdint>

class MemoryManager {
private:
	// 할당된 전체 바이트 수
	static uint32_t TotalAllocationBytes;
	// 할당된 메모리 블록의 개수
	static uint32_t TotalAllocationCount;

public:
	static uint32_t GetTotalAllocationBytes() {
		return TotalAllocationBytes;
	}
	static void SetTotalAllocationBytes(uint32_t bytes) {
		/* FIXME : 추후 관리 로직 추가 */
		TotalAllocationBytes += bytes;
	}
	static uint32_t GetTotalAllocationCount() {
		return TotalAllocationCount;
	}
	static void SetTotalAllocationCount(uint32_t count) {
		/* FIXME : 추후 관리 로직 추가 */
		TotalAllocationCount += count;
	}

};