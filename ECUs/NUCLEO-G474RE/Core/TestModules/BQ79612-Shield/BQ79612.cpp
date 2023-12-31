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

Status DaisyChain::execute(const TransactionContent command[], uint8_t responseFrameCount) {
    resetData();

    writeCommandFrame(command);

    if(HAL_UART_Transmit(handle, buffer, writeBufferPosition, HOST_UART_TIMEOUT) != HAL_OK)
        return Status::ERROR;
    if(HAL_UART_Receive(handle, buffer, receiveBufferSize, HOST_UART_TIMEOUT) != HAL_OK)
        return Status::ERROR;

    readResponseFrames(responseFrameCount);

    return Status::ERROR;
}

Status DaisyChain::readBufferByte(uint8_t *value) {
    if(readBufferPosition >= receiveBufferSize + 1)
        return Status::ERROR;

    *value = buffer[readBufferPosition++];
    checksum.update(*value);

    return Status::COMPLETED;
}

Status DaisyChain::writeBufferByte(uint8_t value) {
    if(writeBufferPosition >= BUFFER_SIZE - 1)
        return Status::ERROR;

    buffer[writeBufferPosition++] = static_cast<uint8_t>(value);
    checksum.update(value);

    return Status::COMPLETED;
}

Status DaisyChain::readBufferWord(uint16_t *value) {
    uint8_t msb;
    uint8_t lsb;

    Status status = readBufferByte(&msb);
    if(status != Status::COMPLETED)
        return Status::ERROR;

    status = readBufferByte(&lsb);
    if(status != Status::COMPLETED)
        return Status::ERROR;

    *value = (static_cast<uint16_t>(msb) << 8) | static_cast<uint16_t>(lsb);

    return Status::COMPLETED;
}

Status DaisyChain::writeBufferWord(uint16_t value) {
    Status status = writeBufferByte(value >> 8);
    if(status != Status::COMPLETED)
        return status;

    status = writeBufferByte(value & 0xFF);
    if(status != Status::COMPLETED)
        return status;

    return Status::COMPLETED;
}

Status DaisyChain::readDataBufferByte(uint8_t *value) {
    if(readDataBufferPosition >= receiveDataBufferSize + 1)
        return Status::ERROR;

    *value = dataBuffer[readDataBufferPosition++];

    return Status::COMPLETED;
}

Status DaisyChain::writeDataBufferByte(uint8_t value) {
    if(writeDataBufferPosition >= DATA_BUFFER_SIZE)
        return Status::ERROR;

    dataBuffer[writeBufferPosition++] = value;

    return Status::COMPLETED;
}

Status DaisyChain::getCommandInit(ReqType requestType, uint8_t dataSize, uint8_t *value) {
    if(dataSize > 8 || dataSize == 0)
        return Status::ERROR;

    *value = (dataSize - 1) | (requestType << 4) | (1 << 7);

    return Status::COMPLETED;
}

Status DaisyChain::readResponseFrames(uint8_t responseFrameCount) {
    Status status = Status::COMPLETED;
    bool endFlag = false;
    uint8_t data;

    for(uint8_t frame = 0; frame < responseFrameCount; frame++){

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
    Status status = Status::COMPLETED;
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

Status DaisyChain::readSingleDevice(uint8_t devAddress, ReadRegister startRegister, uint8_t registerCount) {
    Status status;
    cf_dataLength = 1;
    rf_dataLength = registerCount;
    receiveBufferSize = rf_dataLength + TRANSACTION_FRAME_OVERHEAD;
    receiveDataBufferSize = rf_dataLength;

    status = getCommandInit(ReqType::SINGLE_DEVICE_READ, 1, &cf_init);
    if(status != Status::COMPLETED)
        return status;

    if(devAddress > MAX_DEVICE_COUNT)
        return Status::ERROR;
    else
        cf_devAdr = devAddress;

    cf_regAdr = startRegister;

    writeDataBufferByte(registerCount);

    return execute(SINGLE_DEVICE_COMMAND_FRAME, 1);
}

Status DaisyChain::writeSingleDevice(uint8_t devAddress, WriteRegister startRegister, uint8_t *data, uint8_t dataSize) {
    Status status;
    cf_dataLength = dataSize;
    rf_dataLength = 0;
    receiveBufferSize = 0;
    receiveDataBufferSize = 0;

    status = getCommandInit(ReqType::SINGLE_DEVICE_WRITE, dataSize, &cf_init);
    if(status != Status::COMPLETED)
        return status;

    if(devAddress > MAX_DEVICE_COUNT)
        return Status::ERROR;
    else
        cf_devAdr = devAddress;

    cf_regAdr = startRegister;

    for(uint8_t i = 0; i < dataSize; i++)
        writeDataBufferByte(data[i]);

    return execute(SINGLE_DEVICE_COMMAND_FRAME, 0);
}

/*
Status DaisyChain::readSingleDevice(uint8_t devAddress, ReadOnlyRegister startRegister, uint8_t registerCount) {
    Status status = initCommandFrame(ReqType::SINGLE_DEVICE_READ, 1);
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
}*/