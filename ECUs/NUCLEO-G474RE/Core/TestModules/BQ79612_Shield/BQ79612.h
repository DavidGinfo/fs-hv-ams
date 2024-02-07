#pragma once

#include <util/Common.h>
#include <data/checksum/CRC.h>
#include <parallel/Delay.h>
#include <IO/GPIO/Pin.h>
#include "main.h"
#include "BQ79612_Comm_Data.h"
#include "BQ79612_Register_Data.h"

namespace BQ79612 {

    const uint8_t TRANSACTION_FRAME_OVERHEAD = 6;
    const uint8_t MAX_DATA_SIZE_COMMAND_FRAME = 8;
    const uint8_t MAX_DATA_SIZE_RESPONSE_FRAME = 128;
    const uint8_t MAX_LENGTH_TRANSACTION_FRAME = MAX_DATA_SIZE_RESPONSE_FRAME + TRANSACTION_FRAME_OVERHEAD;
    const uint8_t MAX_DEVICE_COUNT = 64;
    const uint8_t TEST_DEVICE_COUNT = 3;
    //those constants are only valid for testing with the shield (max length of a transaction frame multiplied by the devices of the chain(3 with base device)
    //more generic constants would use huge space in ram (need a better idea for that)
    const uint8_t BUFFER_SIZE = static_cast<uint8_t>(TEST_DEVICE_COUNT * MAX_LENGTH_TRANSACTION_FRAME);
    const uint8_t DATA_BUFFER_SIZE = static_cast<uint8_t>(TEST_DEVICE_COUNT * MAX_DATA_SIZE_RESPONSE_FRAME);
    inline constexpr Parallel::TimeMS HOST_UART_TIMEOUT = 10;

    class DaisyChain{
      public:
        explicit DaisyChain(UART_HandleTypeDef &uartHandle, TIM_HandleTypeDef &timHandle, uint8_t deviceCount, GPIO::Pin uart_tx, GPIO::Pin uart_rx);

        DaisyChain(DaisyChain &) = delete;

        DaisyChain(DaisyChain &&) = delete;

        void init();

        // TODO outsource generic abstraction for hw timer functionality to library
        Status ping(Ping_us time_us);

        Status readDataBuffer(uint8_t *value);

        Status read(ReqType readCommandType, Device device, ReadWriteRegister startRegister, uint8_t registerCount);
        Status read(ReqType readCommandType, Device device, ReadOnlyRegister startRegister, uint8_t registerCount);
        Status write(ReqType writeCommandType, Device device, ReadWriteRegister startRegister, uint8_t *data, uint8_t dataLength);
        Status write(ReqType writeCommandType, Device device, OTPRegister startRegister, uint8_t *data, uint8_t dataLength);

        // TODO implement
        // upper level funcionality
        Status getFaults();
        Status performAutoadressing();

        // TODO implement
        // top level functionality
        Status readCellVoltage();
        Status readCellTemperature();

      private:
        UART_HandleTypeDef *uartHandle;
        TIM_HandleTypeDef *timHandle;
        GPIO::Pin UART_TX;
        GPIO::Pin UART_RX;
        uint8_t deviceCount = 0;

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

        uint8_t rf_init[TEST_DEVICE_COUNT] = {};
        uint8_t rf_devAdr[TEST_DEVICE_COUNT] = {};
        uint16_t rf_regAdr[TEST_DEVICE_COUNT] = {};
        uint16_t rf_crc[TEST_DEVICE_COUNT]    = {};

        uint16_t rf_crc_calc[TEST_DEVICE_COUNT] = {};

        void init_UART_TX_GPIO();

        void resetData();

        Status readBufferByte(uint8_t *value);
        Status writeBufferByte(uint8_t value);

        Status readBufferWord(uint16_t *value);
        Status writeBufferWord(uint16_t value);

        Status readDataBufferByte(uint8_t *value);
        Status writeDataBufferByte(uint8_t value);

        Status readResponseFrames(uint8_t responseFrameCount);
        Status writeCommandFrame(const TransactionContent command[]);

        Status getCommandInit(ReqType requestType, uint8_t dataSize, uint8_t *value);
        Status execute(const TransactionContent command[], uint8_t responseFrameCount);

        Status readRegisters(ReqType readCommandType, Device device, uint16_t startRegister, uint8_t registerCount);
        Status writeRegisters(ReqType writeCommandType, Device device, uint16_t startRegister, uint8_t *data, uint8_t dataLength);
    };
}