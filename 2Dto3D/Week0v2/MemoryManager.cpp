#include "MemoryManager.h"

uint32_t MemoryManager::TotalAllocationBytes = 0;
uint32_t MemoryManager::TotalAllocationCount = 0;

uint32_t MemoryManager::GetTotalAllocationBytes() {
    return TotalAllocationBytes;
}

void MemoryManager::SetTotalAllocationBytes(uint32_t bytes) {
    TotalAllocationBytes += bytes;
}

uint32_t MemoryManager::GetTotalAllocationCount() {
    return TotalAllocationCount;
}

void MemoryManager::SetTotalAllocationCount(uint32_t count) {
    TotalAllocationCount += count;
}
