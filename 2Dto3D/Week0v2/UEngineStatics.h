#pragma once
#include <cstdint>
// UUID생성용 클래스
class UEngineStatics {
public:
	//MOD
	static uint32_t GenUUID();

private:
	static uint32_t NextUUID;
};