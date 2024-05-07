#pragma once

#if defined(MCU_FAMILY_STM32F4)

#include <stm32f4xx_hal.h>
// Fix HAL stupidity
#undef ADC

#elif defined(MCU_FAMILY_STM32G4)

#include <stm32g4xx_hal.h>
// Fix HAL stupidity
#undef ADC

#elif defined(STM32F3)

#include <stm32f3xx_hal.h>
// Fix HAL stupidity
#undef CAN

#elif defined(MCU_FAMILY_STM32H7)

#include <stm32h7xx_hal.h>
// Fix HAL stupidity
#undef ADC

#elif defined(MCU_FAMILY_STM32H7)

#include <stm32h7xx_hal.h>
// Fix HAL stupidity
#undef ADC
#undef CAN


#endif
