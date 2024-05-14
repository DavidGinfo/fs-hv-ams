#pragma once

#include "main.h"

#include "BQ79612_Comm_Data.h"
#include "BQ79612_OTPRegister_Data.h"
#include "BQ79612_ReadOnlyRegister_Data.h"
#include "BQ79612_ReadWriteRegister_Data.h"
#include "BQ79612_Register_Addresses.h"

#include <IO/GPIO/Pin.h>
#include <data/checksum/CRC.h>
#include <parallel/Delay.h>
#include <parallel/notification/BinaryChannel.h>
#include <util/Common.h>
#include <IO/UART/CRXPort.h>

#include <cmath>

namespace BQ79612 {

    const uint8_t TRANSACTION_FRAME_OVERHEAD = 6;
    const uint8_t MAX_DATA_SIZE_COMMAND_FRAME = 8;
    const uint8_t MAX_DATA_SIZE_RESPONSE_FRAME = 128;
    const uint8_t MAX_LENGTH_TRANSACTION_FRAME = MAX_DATA_SIZE_RESPONSE_FRAME + TRANSACTION_FRAME_OVERHEAD;
    const uint8_t MAX_DEVICE_COUNT = 64;
    const uint8_t HV_ACCU_DEVICE_COUNT = 12;
    const uint8_t TEST_DEVICE_COUNT = 3;
    const uint8_t BUFFER_SIZE = static_cast<uint8_t>(HV_ACCU_DEVICE_COUNT * MAX_LENGTH_TRANSACTION_FRAME);
    const uint8_t DATA_BUFFER_SIZE = static_cast<uint8_t>(HV_ACCU_DEVICE_COUNT * MAX_DATA_SIZE_RESPONSE_FRAME);
    inline constexpr Parallel::TimeMS HOST_UART_TIMEOUT = 100;

    const float CELCIUS_KELVIN_OFFSET = 273.15;
    const float FACTOR_MICRO = 1000000;
    const float R_VOLTAGE_DIVIDER = 10000.0;
    const float NTC_B = 3422.0; // optimized for a temperature range of 25°C - 75°C
    const float NTC_T0 = CELCIUS_KELVIN_OFFSET + 25.0;
    const float NTC_R0                = 10000.0;

    const float MAIN_AUX_ADC_RESOLUTION_VCELL = 190.73;
    const float MAIN_AUX_ADC_RESOLUTION_BB = 30.52;
    const float MAIN_ADC_RESOLUTION_DIETEMP1 = 0.025;
    const float AUX_ADC_RESOLUTION_DIETEMP2 = 0.025;
    const float AUX_ADC_RESOLUTION_BAT = 3.05;
    const float MAIN_AUX_ADC_RESOLUTION_GPIO  = 152.59;
    const float MAIN_ADC_RESOLUTION_TSREF = 169.54;

    class DaisyChain{
      public:
        uint8_t deviceCount = 0;

        DaisyChain(DaisyChain &) = delete;

        DaisyChain(DaisyChain &&) = delete;
//
//        explicit DaisyChain(UART_HandleTypeDef &uartHandle, TIM_HandleTypeDef &timHandle, uint8_t deviceCount, GPIO::Pin uart_tx, GPIO::Pin uart_rx);

        //DaisyChain(UART_HandleTypeDef &uartHandle, TIM_HandleTypeDef &timHandle, uint8_t deviceCount, GPIO::Pin uart_tx, GPIO::Pin uart_rx);

        void init(UART_HandleTypeDef &uartHandle, TIM_HandleTypeDef &timHandle, uint8_t deviceCount);

        void handleData(uint8_t *data, uint16_t length, bool idle);

        explicit DaisyChain(UART::CRXPort *uartPort, Parallel::BinaryChannel &channel, GPIO::Pin uart_tx, GPIO::Pin uart_rx);

        void handleUARTInterrupt(UART::CallbackType callbackType);

        UART::CRXPort *getUartCRXPort(){
            return this->uartPort;
        }

        uint8_t getReceiveBufferSize(){
            return this->receiveBufferSize;
        }

        void processDataIfComplete();

        // TODO outsource generic abstraction for hw timer functionality to library
        // TODO implement clean solution for exact delay in us (with rtos and HW timer in combination), commit tim handle via constructor
        Status ping(Ping_us time_us);

        Status write(ReqType writeCommandType, Device device, ReadWriteRegister startRegister, uint8_t *data, uint8_t dataLength);
        Status write(ReqType writeCommandType, Device device, OTPRegister startRegister, uint8_t *data, uint8_t dataLength);
        Status read(ReqType readCommandType, Device device, ReadWriteRegister startRegister, uint8_t registerCount);
        Status read(ReqType readCommandType, Device device, ReadOnlyRegister startRegister, uint8_t registerCount);

        Status readDataBuffer(uint8_t *value);

        void wake();
        void sleep();
        void shutdown();

        void readDeviceIDs();

        Status performAutoadressing();
        Status activateADCs(uint8_t config);
        Status activateVCs(Device device, Active_Cell activeVCConfig);
        Status activateVCs(Active_Cell activeVCConfig);
        Status readCellVoltages(Device device, float *cellVoltages, uint8_t cellVoltageCount);
        Status readCellVoltages(float *cellVoltages, uint8_t cellVoltageCount);
        Status setActivateGPIOADCs(Device device);
        Status setActivateGPIOADCs();
        Status readCellTemperature(Device device, float *cellTemperatures, uint8_t cellTemperatureCount);
        Status readCellTemperature(float *cellTemperatures, uint8_t cellTemperatureCount);
        Status readDieTemperature(float *dieTemperature);
        Status getFaultSummary(Device device, FaultSummary *faults);

//        Status performAutoadressing();
//
//        Status activateADCs(uint8_t config);
//
//        Status activateVCs(Device device, Active_Cell activeVCConfig);
//        Status readCellVoltages(Device device, float *cellVoltages, uint8_t cellVoltageCount);
//
//        Status setActivateGPIOADCs(Device device, uint8_t activeGPIOCount);
//        Status readCellTemperature(Device device, float *cellTemperatures, uint8_t cellTemperatureCount);
//
//        Status readDieTemperature(float *dieTemperature);
//
//        Status getFaultSummary(Device device, FaultSummary *faults);
      private:
        inline static volatile bool done = false;

        uint16_t nextIndex = 0;

        UART_HandleTypeDef *uartHandle;
        TIM_HandleTypeDef *timHandle;
        GPIO::Pin UART_TX;
        GPIO::Pin UART_RX;

        Parallel::BinaryNotification binaryNotification;

        UART::CRXPort *uartPort;

        GPIO_InitTypeDef UART_TX_GPIO_InitStruct;

        Checksum::CRC16Value checksum = Checksum::IBM_AMS.create();

        uint8_t buffer[BUFFER_SIZE] = {};
        uint8_t writeBufferPosition = 0;
        uint8_t readBufferPosition = 0;

        uint8_t dataBuffer[DATA_BUFFER_SIZE] = {};
        uint8_t writeDataBufferPosition = 0;
        uint8_t readDataBufferPosition = 0;

        uint8_t cf_dataLength = 0;
        uint8_t rf_dataLength = 0;
        uint8_t receiveBufferSize = 0;
        uint8_t receiveDataBufferSize = 0;

        uint8_t cf_init = 0;
        uint8_t cf_devAdr = 0;
        uint16_t cf_regAdr = 0;
        uint16_t cf_crc    = 0;

        uint8_t rf_init[HV_ACCU_DEVICE_COUNT] = {};
        uint8_t rf_devAdr[HV_ACCU_DEVICE_COUNT] = {};
        uint16_t rf_regAdr[HV_ACCU_DEVICE_COUNT] = {};
        uint16_t rf_crc[HV_ACCU_DEVICE_COUNT]    = {};

        uint16_t rf_crc_calc[HV_ACCU_DEVICE_COUNT] = {};

        void init_UART_TX_GPIO();

        void resetData();

        Status writeBufferByte(uint8_t value);
        Status readBufferByte(uint8_t *value);
        Status writeBufferWord(uint16_t value);
        Status readBufferWord(uint16_t *value);
        Status writeDataBufferByte(uint8_t value);
        Status readDataBufferByte(uint8_t *value);

        Status writeCommandFrame(const TransactionContent command[]);
        Status readResponseFrames(uint8_t responseFrameCount);

        Status getCommandInit(ReqType requestType, uint8_t dataSize, uint8_t *value);

        Status execute(const TransactionContent command[], uint8_t responseFrameCount);

        Status writeRegisters(ReqType writeCommandType, Device device, uint16_t startRegister, uint8_t *data, uint8_t dataLength);
        Status readRegisters(ReqType readCommandType, Device device, uint16_t startRegister, uint8_t registerCount);
    };
}