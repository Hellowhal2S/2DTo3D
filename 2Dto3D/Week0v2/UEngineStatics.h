#pragma once
#include <cstdint>
// UUID������ Ŭ����
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