#include "BQ79612.h"

using namespace BQ79612;

const TransactionContent SINGLE_DEVICE_COMMAND_FRAME[] = {
        TransactionContent::TC_INIT,
        TransactionContent::TC_DEV_ADR,
        TransactionContent::TC_REG_ADR,
        TransactionContent::TC_DATA,
        TransactionContent::TC_CRC
};

const TransactionContent MULTI_DEVICE_COMMAND_FRAME[] = {
        TransactionContent::TC_INIT,
        TransactionContent::TC_REG_ADR,
        TransactionContent::TC_DATA,
        TransactionContent::TC_CRC
};

const TransactionContent REPSPONSE_FRAME[] = {
        TransactionContent::TC_INIT,
        TransactionContent::TC_DEV_ADR,
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

Status DaisyChain::execute(const TransactionContent command[], uint8_t frameCount) {
    resetData();

    writeCommandFrame(command);

    if(HAL_UART_Transmit(handle, buffer, writeBufferPosition, HOST_UART_TIMEOUT) != HAL_OK)
        return Status::ERROR;
    if(HAL_UART_Receive(handle, buffer, receiveBufferSize, HOST_UART_TIMEOUT) != HAL_OK)
        return Status::ERROR;

    readResponseFrames(frameCount);

    return Status::ERROR;
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

Status DaisyChain::readResponseFrames(uint8_t frameCount) {
    auto status = Status::COMPLETED;
    bool endFlag = false;
    uint8_t data;

    for(uint8_t frame = 0; frame < frameCount; frame++){

        checksum.reset();

        for(uint8_t i = 0; !endFlag; i++){
            switch(REPSPONSE_FRAME[i]){
                case TransactionContent::TC_INIT:
                    status = readBufferByte(&rf_init[frame]);
                    if(status != Status::COMPLETED)
                        return status;

                case TransactionContent::TC_DEV_ADR:
                    status = readBufferByte(&rf_devAdr[frame]);
                    if(status != Status::COMPLETED)
                        return status;

                case TransactionContent::TC_REG_ADR:
                    status = readBufferWord(&rf_regAdr[frame]);
                    if(status != Status::COMPLETED)
                        return status;

                case TransactionContent::TC_DATA:
                    for(uint8_t j = 0; j < rf_dataLength; j++){
                        status = readBufferByte(&data);
                        if(status != Status::COMPLETED)
                            return status;

                        writeDataBufferByte(data);
                    }

                case TransactionContent::TC_CRC:
                    endFlag = true;
                    rf_crc_calc[frame] = swap(checksum.get());
                    status = readBufferWord(&rf_crc[frame]);
                    if(status != Status::COMPLETED)
                        return status;
            }
        }
    }

    return status;
}

Status DaisyChain::writeCommandFrame(const TransactionContent command[]) {
    auto status = Status::COMPLETED;
    bool endFlag = false;

    for(uint8_t i = 0; !endFlag; i++){
        switch(command[i]){
            case TransactionContent::TC_INIT:
                status = writeBufferByte(cf_init);
                if(status != Status::COMPLETED)
                    return status;

            case TransactionContent::TC_DEV_ADR:
                status = writeBufferByte(cf_devAdr);
                if(status != Status::COMPLETED)
                    return status;

            case TransactionContent::TC_REG_ADR:
                status = writeBufferWord(cf_regAdr);
                if(status != Status::COMPLETED)
                    return status;

            case TransactionContent::TC_DATA:
                for(uint8_t j = 0; j < cf_dataLength; j++){
                    status = writeBufferByte(dataBuffer[j]);
                    if(status != Status::COMPLETED)
                        return status;
                }

            case TransactionContent::TC_CRC:
                endFlag = true;
                cf_crc = swap(checksum.get());
                status = writeBufferWord(cf_crc);
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