#pragma once
#include <cstdint>

class MemoryManager {
private:
    static uint32_t TotalAllocationBytes;
    static uint32_t TotalAllocationCount;

public:
    static uint32_t GetTotalAllocationBytes();
    static void SetTotalAllocationBytes(uint32_t bytes);

    static uint32_t GetTotalAllocationCount();
    static void SetTotalAllocationCount(uint32_t count);
};
