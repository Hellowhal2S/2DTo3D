#include "UEngineStatics.h"

uint32_t UEngineStatics::NextUUID = 1;

uint32_t UEngineStatics::GenUUID() {
    return NextUUID++;
}
