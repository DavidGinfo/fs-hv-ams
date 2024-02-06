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
    Parallel::delay(100);
}

void sendShutdownPing(){
    // send wake ping to change device mode from sleep to active
    ams.ping(Ping_us::SHUTDOWN);
}

bool performAutoaddressing(){
    uint8_t data = 0;
    uint8_t dataBuffer[3] = {};

    // send dummy data to sync delay-locked loop
    data |= ~OTP_ECC_Test::DED_SEC | ~OTP_ECC_Test::MANUAL_AUTO | ~OTP_ECC_Test::ENC_DEC | ~OTP_ECC_Test::ENABLE;
    if(ams.broadcastWrite(WriteRegister::OTP_ECC_TEST_W, &data, 1) != Status::COMPLETED)
        return false;
    data = 0;

    // enable autoadressing on all devices
    data |= Control1::ADDR_WR;
    if(ams.broadcastWrite(WriteRegister::Control1_W, &data, 1) != Status::COMPLETED)
        return false;

    data = 0;

    // distribute addresses to all the devices
    for(int i = 0; i < devices; i++){
        data = i;
        if(ams.broadcastWrite(WriteRegister::DIR0_ADDR_W, &data, 1) != Status::COMPLETED)
            return false;
    }
    data = 0;

    // set all devices to stack devices
    data |= COMM_CTRL::STACK_DEV | ~COMM_CTRL::TOP_STACK;
    if(ams.broadcastWrite(WriteRegister::COMM_CTRL_W, &data, 1) != Status::COMPLETED)
        return false;
    data = 0;

    // revoke stack device setting for base device
    data |= ~COMM_CTRL::STACK_DEV | ~COMM_CTRL::TOP_STACK;
    if(ams.writeSingleDevice(0x00, WriteRegister::COMM_CTRL_W, &data, 1) != Status::COMPLETED)
        return false;
    data = 0;

    // set up top of stack device
    data |= COMM_CTRL::STACK_DEV | COMM_CTRL::TOP_STACK;
    if(ams.writeSingleDevice(0x02, WriteRegister::COMM_CTRL_W, &data, 1) != Status::COMPLETED)
        return false;
    data = 0;

    // read dummy data to sync delay-locked loop
    if(ams.broadcastRead(ReadRegister::OTP_ECC_TEST_R, 1) != Status::COMPLETED)
        return false;

    // read back device addresses of all devices
    if(ams.broadcastRead(ReadRegister::DIR0_ADDR_R, 1) != Status::COMPLETED)
        return false;

    for(int i = 0; i < devices; i++){
        ams.readData(&dataBuffer[i]);
        Terminal::printf("0x0%d\r\n", dataBuffer[i]);
    }

    return true;
}

bool readBaseDeviceID(){
    uint8_t data;
    uint8_t expectedBaseDeviceAddress = 0x00; // factory default config, if this will not work try 0x00 which is the HW reset default address
    if(ams.readSingleDevice(expectedBaseDeviceAddress,
                             ReadRegister::PART_ID, 1) != Status::COMPLETED)
        return false;

    ams.readData(&data);

    return true;
}

void runAMS_Shield(){
    bool status = true;

    ams.init();

    sendShutdownPing();
    sendWakePing();
    sendWakePing();

    // broadcast write goto shutdown -> worked
//    uint8_t buffersdrpid[6] = {0xD0, 0x03, 0x09, 0x08, 0xcf, 0x72}; // D0 03 09 08 CF 72
//    if(HAL_UART_Transmit(&huart4, buffersdrpid, 7, 10) != HAL_OK)
//        status = false;

    // hardcoded autoadressing

    uint8_t buffer6_1[6] = {0xD0, 0x03, 0x4C, 0x00, 0xFC, 0x24}; // D0 03 4C 00 FC 24
    if(HAL_UART_Transmit(&huart4, buffer6_1, 6, 10) != HAL_OK)
        status = false;

    uint8_t buffer6_2[6] = {0xD0, 0x03, 0x09, 0x01, 0x0F, 0x74}; // D0 03 09 01 0F 74
    if(HAL_UART_Transmit(&huart4, buffer6_2, 6, 100) != HAL_OK)
        status = false;

    uint8_t buffer6_3[6] = {0xD0, 0x03, 0x06, 0x00, 0xCB, 0x44}; // D0 03 06 00 CB 44
    if(HAL_UART_Transmit(&huart4, buffer6_3, 6, 10) != HAL_OK)
        status = false;

    uint8_t buffer6_4[6] = {0xD0, 0x03, 0x06, 0x01, 0x0A, 0x84}; // D0 03 06 01 0A 84
    if(HAL_UART_Transmit(&huart4, buffer6_4, 6, 10) != HAL_OK)
        status = false;

    uint8_t buffer6_5[6] = {0xD0, 0x03, 0x06, 0x02, 0x4A, 0x85}; // D0 03 06 02 4A 85
    if(HAL_UART_Transmit(&huart4, buffer6_5, 6, 10) != HAL_OK)
        status = false;

    uint8_t buffer6_6[6] = {0xD0, 0x03, 0x08, 0x02, 0x4E, 0xE5}; // D0 03 08 02 4E E5
    if(HAL_UART_Transmit(&huart4, buffer6_6, 6, 10) != HAL_OK)
        status = false;

    uint8_t buffer7_7[7] = {0x90, 0x00, 0x03, 0x08, 0x00, 0x13, 0xDD}; // 90 00 03 08 00 13 DD
    if(HAL_UART_Transmit(&huart4, buffer7_7, 7, 10) != HAL_OK)
        status = false;

    uint8_t buffer7_8[7] = {0x90, 0x02, 0x03, 0x08, 0x03, 0x52, 0x64}; // 90 02 03 08 03 52 64
    if(HAL_UART_Transmit(&huart4, buffer7_8, 7, 10) != HAL_OK)
        status = false;

    uint8_t buffer6_9[6] = {0xC0, 0x03, 0x4C, 0x00, 0xF8, 0xE4}; // C0 03 4C 00 F8 E4
    if(HAL_UART_Transmit(&huart4, buffer6_9, 6, 10) != HAL_OK)
        status = false;
    uint8_t buffer6_10[18] = {};
    if(HAL_UART_Receive(&huart4, buffer6_10, 21, 100) != HAL_OK)
        status = false;


    uint8_t buffersr[6] = {0xA0, 0x03, 0x06, 0x00, 0xD1, 0x84}; // A0 03 06 00 D1 84
    if(HAL_UART_Transmit(&huart4, buffersr, 6, 10) != HAL_OK)
        status = false;
    uint8_t buffersre[14] = {};
    if(HAL_UART_Receive(&huart4, buffersre, 14, 100) != HAL_OK)
        status = false;

    for(;;){
        Parallel::delay(10);
    }
}

void initAMS_Shield(){
    amsShieldTask.init();
}