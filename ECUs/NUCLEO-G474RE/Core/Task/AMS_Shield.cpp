#include <parallel/Task.hpp>
#include <util/Terminal.h>

#include "Init.h"
#include "main.h"
#include "../TestModules/BQ79612_Shield/BQ79612.h"

using namespace BQ79612;

extern UART_HandleTypeDef huart4;
extern TIM_HandleTypeDef htim16;

[[noreturn]] void runAMS_Shield();

Parallel::NormalTask amsShieldTask(&runAMS_Shield, "AMS-Shield");

GPIO::Pin uart_tx = GPIO::Pin(Host_UART_TX_GPIO_Port, Host_UART_TX_Pin);
GPIO::Pin uart_rx = GPIO::Pin(Host_UART_RX_GPIO_Port, Host_UART_RX_Pin);

uint8_t devices = 3;

DaisyChain ams(huart4, htim16, devices, uart_tx, uart_rx);

void sendWakePing(){
    // send wake ping to change device mode from sleep to active
    ams.ping(Ping_us::WAKE);

    // wait min 10ms + 600us per device wake propagation
    Parallel::delay(15);
}

bool performAutoaddressing(){
    uint8_t data = 0;
    uint8_t dataBuffer[3] = {};

    // send dummy data to sync delay-locked loop
    data |= ~OTP_ECC_Test::DED_SEC | ~OTP_ECC_Test::MANUAL_AUTO | ~OTP_ECC_Test::ENC_DEC | ~OTP_ECC_Test::ENABLE;
    if(ams.broadcastWrite(OTP_ECC_TEST_W, &data, 1) != Status::COMPLETED)
        return false;
    data = 0;

    // enable autoadressing on all devices
    data |= Control1::ADDR_WR;
    if(ams.broadcastWrite(Control1_W, &data, 1) != Status::COMPLETED)
        return false;

    data = 0;

    // distribute addresses to all the devices
    for(int i = 0; i < devices; i++){
        data = i;
        if(ams.broadcastWrite(DIR0_ADDR_W, &data, 1) != Status::COMPLETED)
            return false;
    }
    data = 0;

    // set all devices to stack devices
    data |= COMM_CTRL::STACK_DEV | ~COMM_CTRL::TOP_STACK;
    if(ams.broadcastWrite(COMM_CTRL_W, &data, 1) != Status::COMPLETED)
        return false;
    data = 0;

    // revoke stack device setting for base device
    data |= ~COMM_CTRL::STACK_DEV | ~COMM_CTRL::TOP_STACK;
    if(ams.writeSingleDevice(0x00, COMM_CTRL_W, &data, 1) != Status::COMPLETED)
        return false;
    data = 0;

    // set up top of stack device
    data |= COMM_CTRL::STACK_DEV | COMM_CTRL::TOP_STACK;
    if(ams.writeSingleDevice(0x02, COMM_CTRL_W, &data, 1) != Status::COMPLETED)
        return false;
    data = 0;

    // read dummy data to sync delay-locked loop
    if(ams.broadcastRead(OTP_ECC_TEST_R, 1) != Status::COMPLETED)
        return false;

    // read back device addresses of all devices
    if(ams.broadcastRead(DIR0_ADDR_R, 1) != Status::COMPLETED)
        return false;

    for(int i = 0; i < devices; i++){
        ams.readData(&dataBuffer[i]);
        Terminal::printf("0x0%d\r\n", dataBuffer[i]);
    }

    return true;
}

// TODO implement a simple read reg function for testing if that is possible without autoadressing
bool readBaseDeviceRegister(){

}

void runAMS_Shield(){

    ams.init();

    sendWakePing();

    if(performAutoaddressing())
        Terminal::printf("Looks like it worked!");

    for(;;){

    }
}

void initAMS_Shield(){
    uint8_t a = 0;
    amsShieldTask.init();
}