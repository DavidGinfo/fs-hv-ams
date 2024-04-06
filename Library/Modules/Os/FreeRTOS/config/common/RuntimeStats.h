#pragma once

#include <stdint.h>

#if RUNTIME_STATS
#ifdef __cplusplus
extern "C" {
#endif
void configureTimerForRuntimeStats();
uint32_t getRuntimeStatsCounterValue();
#ifdef __cplusplus
}
#endif

#define configGENERATE_RUN_TIME_STATS       1
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() configureTimerForRuntimeStats()
#define portGET_RUN_TIME_COUNTER_VALUE() getRuntimeStatsCounterValue()

#endif
