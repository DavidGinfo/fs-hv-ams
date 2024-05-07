#include "main.h"
#include "Task/Init.h"
#include <library.h>
#include <parallel/RTOS.h>
#include <IO/CAN/generated/CANMessages.h>


uint8_t aliveValue = 0;

void onSendState() {
    FCU_State::FCU_Alive.setRawValue(aliveValue++);
}

int main() {
    initHAL();
    library::init();

    FCU_State::message.setHandler(&onSendState);

    initADC();
    initFuses();

    RTOS::start();
}
