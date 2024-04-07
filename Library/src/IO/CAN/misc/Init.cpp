#include <IO/CAN/Init.h>
#include <IO/CAN/Peripheral.h>
#include <IO/CAN/Timer.h>
#include <IO/CAN/generated/CANTask.h>


namespace CAN {
  void init() {
      initBuses();
      initPeripherals();
      initTimer();
      initManagers();
  }
}
