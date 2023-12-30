// TODO more generic version to send read/write etc. statements (sth. like Schwärzle did with Commandcontent arrays)

#include "BQ79612.h"

using namespace BQ79612;

Status DaisyChain::readSingleDevice(uint8_t devAddress, ReadOnlyRegister startRegister, uint8_t registerCount) {
    auto status = initCommandFrame(ReqType::SINGLE_DEVICE_READ, 1);
    if(status != Status::COMPLETED)
        return status;

    if(devAddress > deviceCount - 1)
        return Status::ERROR;

    status = writeBufferByte(devAddress);
    if(status != Status::COMPLETED)
        return status;

    status = writeBufferWord(startRegister);
    if(status != Status::COMPLETED)
        return status;

    status = writeBufferByte(registerCount);
    if(status != Status::COMPLETED)
        return status;

    status = appendCRCBytes();
    if(status != Status::COMPLETED)
        return status;

    return Status::COMPLETED;
}
Status DaisyChain::initCommandFrame(ReqType requestType, uint8_t dataSize) {
    if(dataSize > 8 || dataSize == 0)
        return Status::ERROR;

    uint8_t data = (dataSize - 1) | (requestType << 4) | (1 << 7);

    return writeBufferByte(data);
}
Status DaisyChain::writeBufferByte(uint8_t value) {
    if(writeBufferPosition >= BUFFER_SIZE - 1)
        return Status::ERROR;

    buffer[writeBufferPosition++] = static_cast<uint8_t>(value);

    checksum.update(value);
    return Status::COMPLETED;
}
Status DaisyChain::writeBufferWord(uint16_t value) {
    auto status = writeBufferByte(value >> 8);
    if(status != Status::COMPLETED)
        return status;

    status = writeBufferByte(value & 0xFF);
    if(status != Status::COMPLETED)
        return status;

    return Status::COMPLETED;
}
Status DaisyChain::appendCRCBytes() {
    actualChecksum = swap(checksum.get());

    return writeBufferWord(actualChecksum);
}
void DaisyChain::resetData() {
    checksum.reset();
    writeBufferPosition = 0;
    readBufferPosition = 0;
}

Status DaisyChain::execute() {
    if(HAL_UART_Transmit(handle, buffer, writeBufferPosition, HOST_UART_TIMEOUT) != HAL_OK)
        return Status::ERROR;
    if(HAL_UART_Receive(handle, buffer, readBufferPosition, HOST_UART_TIMEOUT) != HAL_OK)
        return Status::ERROR;

    resetData();

    return Status::COMPLETED;
}