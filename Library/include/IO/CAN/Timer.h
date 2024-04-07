#pragma once
#include <util/Common.h>

#define CAN_TIMER_PERIOD 1

namespace CAN {
  inline constexpr uint32_t TIMER_MAX_VALUE = 0xFFFFFFFF;
  
  /**
   * Initializes the CAN timer. Do not call manually.
   */
  void initTimer();
  
  /**
   * @return the current CAN timestamp
   */
  uint32_t getTimestamp();
}
