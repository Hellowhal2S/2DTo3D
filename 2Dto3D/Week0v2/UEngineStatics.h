#pragma once
#include <cstdint>
// UUID생성용 클래스
class UEngineStatics {
public:
	static uint32_t GenUUID()
	{
		return NextUUID++;
	}

private:
	static uint32_t NextUUID;
};
uint32_t UEngineStatics::NextUUID = 1;