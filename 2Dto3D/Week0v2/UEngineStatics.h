#pragma once
#include <cstdint>
// UUID������ Ŭ����
class UEngineStatics {
public:
	//MOD
	static uint32_t GenUUID();

private:
	static uint32_t NextUUID;
};