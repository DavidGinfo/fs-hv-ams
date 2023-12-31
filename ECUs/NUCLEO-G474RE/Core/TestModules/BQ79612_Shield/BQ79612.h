#pragma once

#include <util/Common.h>
#include <data/checksum/CRC.h>
#include <parallel/Delay.h>
#include <IO/GPIO/Pin.h>
#include "main.h"

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

    enum TransactionContent {
        TC_INIT,
        TC_DEV_ADR,
        TC_REG_ADR,
        TC_DATA,
        TC_CRC
    };

    enum ReqType : uint8_t {
        SINGLE_DEVICE_READ = 0,
        SINGLE_DEVICE_WRITE = 1,
        STACK_READ = 2,
        STACK_WRITE = 3,
        BROADCAST_READ = 4,
        BROADCAST_WRITE = 5,
        BROADCAST_WRITE_REVERSE = 6,
    };

    enum Ping_us : uint16_t {
        WAKE = 2250,
        SHUTDOWN = 8500,
        SLEEP_TO_ACTIVE = 275,
        HARDWARE_RESET = 36
    };

    // TODO find better and hierachical structure -> sth like Readregister > readonly and readwrite > register > register bits
    enum ReadRegister : uint16_t {
        OTP_ECC_TEST_R = 0x034C,
        Control1_R = 0x0309,
        DIR0_ADDR_R = 0x0306,
        COMM_CTRL_R = 0x0308
    };
    enum WriteRegister : uint16_t {
        OTP_ECC_TEST_W = 0x034C,
        Control1_W = 0x0309,
        DIR0_ADDR_W = 0x0306,
        COMM_CTRL_W = 0x0308
    };

    enum Control1 : uint8_t {
        DIR_SEL = 1 << 7,
        SEND_SHUTDOWN = 1 << 6,
        SEND_WAKE = 1 << 5,
        SEND_SLPTOACT = 1 << 4,
        GOTO_SHUTDOWN = 1 << 3,
        GOTO_SLEEP = 1 << 2,
        SOFT_RESET = 1 << 1,
        ADDR_WR = 1
    };

    enum OTP_ECC_Test : uint8_t {
        DED_SEC = 1 << 3,
        MANUAL_AUTO = 1 << 2,
        ENC_DEC = 1 << 1,
        ENABLE = 1,
    };

    enum COMM_CTRL : uint8_t {
        STACK_DEV = 1 << 1,
        TOP_STACK = 1
    };

    enum Status : uint8_t {
        ERROR = 0,
        COMPLETED = 1
    };

    class DaisyChain{
      public:
        explicit DaisyChain(UART_HandleTypeDef &uartHandle, TIM_HandleTypeDef &timHandle, uint8_t deviceCount, GPIO::Pin uart_tx, GPIO::Pin uart_rx);

        DaisyChain(DaisyChain &) = delete;

        DaisyChain(DaisyChain &&) = delete;

        void init();

        Status ping(Ping_us time_us);

        Status readData(uint8_t *value);

        Status readSingleDevice(uint8_t devAddress, ReadRegister startRegister, uint8_t registerCount);

        Status writeSingleDevice(uint8_t devAddress, WriteRegister startRegister, uint8_t *data, uint8_t dataSize);

        Status readStack(ReadRegister startRegister, uint8_t registerCount);

        Status writeStack(WriteRegister startRegister, uint8_t *data, uint8_t dataSize);

        Status broadcastRead(ReadRegister startRegister, uint8_t registerCount);

        Status broadcastWrite(WriteRegister startRegister, uint8_t *data, uint8_t dataSize);

        Status broadcastWriteReverse(WriteRegister startRegister);

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

        Status assignTransactionParamters();
    };
}