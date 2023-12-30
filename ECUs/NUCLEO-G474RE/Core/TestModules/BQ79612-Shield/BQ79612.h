#pragma once

#include <util/Common.h>
#include <data/checksum/CRC.h>
#include <parallel/Delay.h>

namespace BQ79612 {

    const uint8_t TRANSACTION_FRAME_OVERHEAD = 6;
    const uint8_t MAX_DATA_SIZE_COMMAND_FRAME = 8;
    const uint8_t MAX_DATA_SIZE_RESPONSE_FRAME = 128;
    const uint8_t MAX_LENGTH_TRANSACTION_FRAME = 128 + 6;
    const uint8_t MAX_DEVICE_COUNT = 64;
    //those constants are only valid for testing with the shield (max length of a transaction frame multiplied by the devices of the chain(3 with base device)
    //more generic constants would use huge space in ram (need a better idea for that)
    const uint8_t BUFFER_SIZE = 3 * MAX_LENGTH_TRANSACTION_FRAME;
    const uint8_t DATA_BUFFER_SIZE = 2 * MAX_DATA_SIZE_RESPONSE_FRAME;
    Parallel::TimeMS HOST_UART_TIMEOUT = 10;


    enum ReqType : uint8_t {
        SINGLE_DEVICE_READ = 0,
        SINGLE_DEVICE_WRITE = 1,
        STACK_READ = 2,
        STACK_WRITE = 3,
        BROADCAST_READ = 4,
        BROADCAST_WRITE = 5,
        BROADCAST_WRITE_REVERSE = 6,
    };

    enum ReadOnlyRegister : uint16_t {

    };

    enum Status : uint8_t {
        ERROR = 0,
        COMPLETED = 1
    };

    class DaisyChain{
      public:
        Status readSingleDevice(uint8_t devAddress, ReadOnlyRegister startRegister, uint8_t registerCount);

        Status writeSingleDevice();

        Status readStack();

        Status writeStack();

        Status broadcastRead();

        Status broadcastWrite();

        Status broadcastWriteReverse();

      private:
        UART_HandleTypeDef *handle;
        uint8_t deviceCount = 0;

        uint8_t buffer[BUFFER_SIZE] = {};
        uint8_t dataBuffer[DATA_BUFFER_SIZE] = {};
        uint8_t writeBufferPosition = 0;
        uint8_t readBufferPosition = 0;

        Checksum::CRC16Value checksum = Checksum::IBM_AMS.create();
        uint16_t actualChecksum = 0;

        Status writeBufferByte(uint8_t value);
        Status writeBufferWord(uint16_t value);
        Status initCommandFrame(ReqType requestType, uint8_t dataSize);
        Status appendCRCBytes();
        void resetData();
        Status execute();
    };
}
