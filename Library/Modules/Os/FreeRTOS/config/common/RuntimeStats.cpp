#if RUNTIME_STATS
#include <RuntimeStats.h>

#include <util/HAL.h>

#if ENABLE_SYSTEM_VIEW
#error SystemView and CLion runtime statistics are incompatible, use SystemView to get the runtime statistics instead
#endif

#if defined(STM32F4)
TIM_HandleTypeDef        htim13;

void configureTimerForRuntimeStats() {
    uint32_t baseClockSpeed = 2 * HAL_RCC_GetPCLK1Freq();
    
    htim13.Instance = TIM13;
    htim13.Init.Period = 0xFFFF;
    htim13.Init.Prescaler = (uint32_t) ((baseClockSpeed / 50000U) - 1U); // Recommended being 10-100 time tick speed, it is 1 kHz -> 50 kHz
    htim13.Init.ClockDivision = 0;
    htim13.Init.CounterMode = TIM_COUNTERMODE_UP;
    
    __HAL_RCC_TIM13_CLK_ENABLE();
    HAL_TIM_Base_Init(&htim13);
    HAL_TIM_Base_Start(&htim13);
}

uint32_t getRuntimeStatsCounterValue() {
    return TIM13->CNT;
}
#elif defined(STM32G4)
TIM_HandleTypeDef        htim13;

void configureTimerForRuntimeStats() {
    uint32_t baseClockSpeed = 2 * HAL_RCC_GetPCLK1Freq();

    htim13.Instance = TIM15;
    htim13.Init.Period = 0xFFFF;
    htim13.Init.Prescaler = (uint32_t) ((baseClockSpeed / 50000U) - 1U); // Recommended being 10-100 time tick speed, it is 1 kHz -> 50 kHz
    htim13.Init.ClockDivision = 0;
    htim13.Init.CounterMode = TIM_COUNTERMODE_UP;

    __HAL_RCC_TIM15_CLK_ENABLE();
    HAL_TIM_Base_Init(&htim13);
    HAL_TIM_Base_Start(&htim13);
}

uint32_t getRuntimeStatsCounterValue() {
    return TIM15->CNT;
}
#else

#error Runtime stats implementation does not exist for this microcontroller
#endif

#endif
