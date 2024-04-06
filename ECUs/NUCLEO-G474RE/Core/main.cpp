#include "main.h"
#include "Task/Init.h"

#include <library.h>
#include <parallel/RTOS.h>
#include <IO/CAN/generated/CAN1_dbc.h>


uint8_t alive = 0;

void onSendState(){
    alive += 1;
//    AMS_Simulink::Min_Temperature.setRawValue(alive);
}

int main() {
    initHAL();
    library::init();

    initAMS_Shield();

//    AMS_Simulink::message.setHandler(&onSendState);

    RTOS::start();
}
