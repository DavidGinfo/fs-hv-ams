#pragma once


#include <IO/GPIO/Interrupt.h>

#define HAL_GPIO_EXTI_IRQHandler(pin)         \
    do {                                      \
        auto previousPin = GPIO::activePin;   \
        GPIO::activePin = GPIO::linePin<pin>; \
        HAL_GPIO_EXTI_IRQHandler(pin);        \
        GPIO::activePin = previousPin;        \
    } while (false)

#if FEATURE_DMA

#include <IO/DMA/Interrupt.h>
#include <util.h>

namespace DMA {
  force_inline void *setPort(DMA_HandleTypeDef *handle) {
      if (handle->Init.*ACTIVE_MEMBER != ACTIVE_VALUE)
          return nullptr;
      
      auto previous = reinterpret_cast<void **>(handle->Init.*TARGET_MEMBER);
      *reinterpret_cast<void **>(handle->Init.*TARGET_MEMBER) = reinterpret_cast<void *>(handle->Init.*VALUE_MEMBER);
      return previous;
  }
  
  force_inline void resetPort(DMA_HandleTypeDef *handle, void *previous) {
      if (handle->Init.*ACTIVE_MEMBER == ACTIVE_VALUE)
          *reinterpret_cast<void **>(handle->Init.*TARGET_MEMBER) = previous;
  }
}

#define HAL_DMA_IRQHandler(handle)                 \
    do {                                           \
        auto previousPort = DMA::setPort(handle);  \
        HAL_DMA_IRQHandler(handle);                \
        DMA::resetPort(handle, previousPort);      \
    } while (false)

#endif

#if FEATURE_I2C

#include <IO/I2C/Interrupt.h>

#define HAL_I2C_EV_IRQHandler(handle)              \
    do {                                           \
        auto previousPort = I2C::activePort;       \
        I2C::activePort = I2C::handlePort<handle>; \
        HAL_I2C_EV_IRQHandler(handle);             \
        I2C::activePort = previousPort;            \
    } while (false)

#define HAL_I2C_ER_IRQHandler(handle)              \
    do {                                           \
        auto previousPort = I2C::activePort;       \
        I2C::activePort = I2C::handlePort<handle>; \
        HAL_I2C_ER_IRQHandler(handle);             \
        I2C::activePort = previousPort;            \
    } while (false)

#endif

#if FEATURE_SPI

#include <IO/SPI/Interrupt.h>

#define HAL_SPI_IRQHandler(handle)                 \
    do {                                           \
        auto previousPort = SPI::activePort;       \
        SPI::activePort = SPI::handlePort<handle>; \
        HAL_SPI_IRQHandler(handle);                \
        SPI::activePort = previousPort;            \
    } while (false)

#endif

#if FEATURE_UART

#include <IO/UART/Interrupt.h>

#define HAL_UART_IRQHandler(handle)                  \
    do {                                             \
        auto previousPort = UART::activePort;        \
        UART::activePort = UART::handlePort<handle>; \
        HAL_UART_IRQHandler(handle);                 \
        UART::handleIRQ(UART::handlePort<handle>);   \
        UART::activePort = previousPort;             \
    } while (false)

#endif
