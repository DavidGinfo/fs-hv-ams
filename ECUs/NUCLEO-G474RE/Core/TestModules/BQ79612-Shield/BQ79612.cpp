#include "BQ79612.h"

using namespace BQ79612;

const TransactionContent SINGLE_DEVICE[] = {
        TransactionContent::TC_INIT,
        TransactionContent::TC_DEV_ADR,
        TransactionContent::TC_REG_ADR,
        TransactionContent::TC_DATA,
        TransactionContent::TC_CRC
};

const TransactionContent MULTI_DEVICE[] = {
        TransactionContent::TC_INIT,
        TransactionContent::TC_REG_ADR,
        TransactionContent::TC_DATA,
        TransactionContent::TC_CRC
};

void DaisyChain::init_UART_TX_GPIO() {
    UART_TX_GPIO_InitStruct.Pin = UART_TX.getPin();
    UART_TX_GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    UART_TX_GPIO_InitStruct.Pull = GPIO_PULLUP;
    UART_TX_GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
}

void DaisyChain::init() {
    init_UART_TX_GPIO();
}

void DaisyChain::resetData() {
    checksum.reset();
    writeBufferPosition = 0;
    readBufferPosition = 0;
    writeDataBufferPosition = 0;
    readDataBufferPosition = 0;
}

Status DaisyChain::execute(const TransactionContent command[]) {
    resetData();

    writeCommand(command);

    if(HAL_UART_Transmit(handle, buffer, writeBufferPosition, HOST_UART_TIMEOUT) != HAL_OK)
        return Status::ERROR;
    // TODO think about read data buffer length
    if(HAL_UART_Receive(handle, buffer, readBufferPosition, HOST_UART_TIMEOUT) != HAL_OK)
        return Status::ERROR;

    return readCommand(command);
}

// TODO implement
Status DaisyChain::readBufferByte(uint8_t *value) {
    return ERROR;
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

// TODO implement
Status DaisyChain::readBufferWord(uint16_t *value) {
    return ERROR;
}

Status DaisyChain::readDataBufferByte(uint8_t value) {
    if(writeDataBufferPosition >= DATA_BUFFER_SIZE)
        return Status::ERROR;

    dataBuffer[writeBufferPosition++] = value;

    return Status::COMPLETED;
}

// TODO implement
Status DaisyChain::writeDataBufferByte(uint8_t value) {
    return ERROR;
}

// TODO see if this is still necessary
Status DaisyChain::initCommandFrame(ReqType requestType, uint8_t dataSize) {
    if(dataSize > 8 || dataSize == 0)
        return Status::ERROR;

    uint8_t data = (dataSize - 1) | (requestType << 4) | (1 << 7);

    return writeBufferByte(data);
}

// TODO implement
Status DaisyChain::readCommand(const TransactionContent command[]) {
    return Status::COMPLETED;
}

Status DaisyChain::writeCommand(const TransactionContent command[]) {
    auto status = Status::COMPLETED;
    bool endFlag = false;

    for(uint8_t i = 0; !endFlag; i++){
        switch(command[i]){
            case TransactionContent::TC_INIT:
                status = writeBufferByte(tc_init);
                if(status != Status::COMPLETED)
                    return status;

            case TransactionContent::TC_DEV_ADR:
                status = writeBufferByte(tc_devAdr);
                if(status != Status::COMPLETED)
                    return status;

            case TransactionContent::TC_REG_ADR:
                status = writeBufferWord(tc_regAdr);
                if(status != Status::COMPLETED)
                    return status;

            case TransactionContent::TC_DATA:
                for(uint8_t j = 0; j < dataLength; j++){
                    status = writeBufferByte(dataBuffer[j]);
                    if(status != Status::COMPLETED)
                        return status;
                }

            case TransactionContent::TC_CRC:
                endFlag = true;
                status = writeBufferWord(swap(checksum.get()));
                if(status != Status::COMPLETED)
                    return status;
        }
    }

    return status;
}

// implement clean solution for exact delay in us (with rtos and HW timer in combination)
Status DaisyChain::ping(uint16_t time_us) {
    HAL_UART_DeInit(handle);

    HAL_GPIO_Init(UART_TX.getPort(), &UART_TX_GPIO_InitStruct);

    if(UART_TX.read() != 1)
        return Status::ERROR;

    UART_TX.write(0);
    HAL_TIM_Base_Start(&htim16);
    uint16_t startTime = __HAL_TIM_GET_COUNTER(&htim16);
    while(__HAL_TIM_GET_COUNTER(&htim16) - startTime >= time_us);
    UART_TX.write(1);

    HAL_GPIO_DeInit(UART_TX.getPort(), UART_TX.getPin());

    HAL_UART_Init(handle);

    return Status::COMPLETED;
}

// TODO implement with new driver architecture
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

    // TODO: implement write data function with writeDataBufferPosition++ which automatically adds the data to the buffer AND databuffer
    status = writeBufferByte(registerCount - 1);
    if(status != Status::COMPLETED)
        return status;

    status = appendCRCBytes();
    if(status != Status::COMPLETED)
        return status;

    readBufferPosition = TRANSACTION_FRAME_OVERHEAD + registerCount;
    readDataBufferPosition = registerCount;

    //status = execute();
    if(status != Status::COMPLETED)
        return status;

    // TODO actual read: write data bytes in data buffer readByte/Word/Data function

    return Status::COMPLETED;
}