#pragma once

#include <stdint.h>

void otaBegin();
void otaHandle();
void otaDelay(uint32_t durationMs);
bool otaIsReady();
