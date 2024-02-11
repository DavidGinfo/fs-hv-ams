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

const TransactionContent RESPONSE_FRAME[] = {
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

void DaisyChain::resetData() {
    checksum.reset();
    writeBufferPosition = 0;
    readBufferPosition = 0;
    writeDataBufferPosition = 0;
    readDataBufferPosition = 0;
}

Status DaisyChain::writeBufferByte(uint8_t value) {
    if(writeBufferPosition >= BUFFER_SIZE - 1)
        return Status::ERROR;

    buffer[writeBufferPosition++] = static_cast<uint8_t>(value);
    checksum.update(value);

    return Status::COMPLETED;
}

Status DaisyChain::readBufferByte(uint8_t *value) {
    if(readBufferPosition >= receiveBufferSize + 1)
        return Status::ERROR;

    *value = buffer[readBufferPosition++];
    checksum.update(*value);

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

Status DaisyChain::writeDataBufferByte(uint8_t value) {
    if(writeDataBufferPosition >= DATA_BUFFER_SIZE)
        return Status::ERROR;

    dataBuffer[writeDataBufferPosition++] = value;

    return Status::COMPLETED;
}

Status DaisyChain::readDataBufferByte(uint8_t *value) {
    if(readDataBufferPosition >= receiveDataBufferSize + 1)
        return Status::ERROR;

    *value = dataBuffer[readDataBufferPosition++];

    return Status::COMPLETED;
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
                break;

            case TransactionContent::TC_DEV_ADR:
                status = writeBufferByte(cf_devAdr);
                if(status != Status::COMPLETED)
                    return status;
                break;

            case TransactionContent::TC_REG_ADR:
                status = writeBufferWord(cf_regAdr);
                if(status != Status::COMPLETED)
                    return status;
                break;

            case TransactionContent::TC_DATA:
                for(uint8_t j = 0; j < cf_dataLength; j++){
                    status = writeBufferByte(dataBuffer[j]);
                    if(status != Status::COMPLETED)
                        return status;
                }
                break;

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

Status DaisyChain::readResponseFrames(uint8_t responseFrameCount) {
    Status status = Status::COMPLETED;
    bool endFlag;
    uint8_t data;

    for(uint8_t frame = 0; frame < responseFrameCount; frame++){

        endFlag = false;
        checksum.reset();

        for(uint8_t i = 0; !endFlag; i++){
            switch(RESPONSE_FRAME[i]){
                case TransactionContent::TC_INIT:
                    status = readBufferByte(&rf_init[frame]);
                    if(status != Status::COMPLETED)
                        return status;
                    break;

                case TransactionContent::TC_DEV_ADR:
                    status = readBufferByte(&rf_devAdr[frame]);
                    if(status != Status::COMPLETED)
                        return status;
                    break;

                case TransactionContent::TC_REG_ADR:
                    status = readBufferWord(&rf_regAdr[frame]);
                    if(status != Status::COMPLETED)
                        return status;
                    break;

                case TransactionContent::TC_DATA:
                    for(uint8_t j = 0; j < rf_dataLength; j++){
                        status = readBufferByte(&data);
                        if(status != Status::COMPLETED)
                            return status;

                        writeDataBufferByte(data);
                    }
                    break;

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

Status DaisyChain::getCommandInit(ReqType requestType, uint8_t dataSize, uint8_t *value) {
    if(dataSize > 8 || dataSize == 0)
        return Status::ERROR;

    *value = (dataSize - 1) | (static_cast<uint8_t>(requestType) << 4) | (1 << 7);

    return Status::COMPLETED;
}

Status DaisyChain::execute(const TransactionContent command[], uint8_t responseFrameCount) {
    resetData();

    writeCommandFrame(command);
    if(HAL_UART_Transmit(uartHandle, buffer, writeBufferPosition, HOST_UART_TIMEOUT) != HAL_OK){
        return Status::ERROR;
    }

    if(responseFrameCount > 0){
        if(HAL_UART_Receive(uartHandle, buffer, receiveBufferSize, HOST_UART_TIMEOUT) != HAL_OK){
            return Status::ERROR;
        }
        readResponseFrames(responseFrameCount);
    }

    resetData();

    return Status::COMPLETED;
}

Status DaisyChain::writeRegisters(ReqType writeCommandType, Device device, uint16_t startRegister, uint8_t *data, uint8_t dataLength) {
    Status status = getCommandInit(writeCommandType, dataLength, &cf_init);
    if(status != Status::COMPLETED){
        return status;
    }

    cf_dataLength = dataLength;
    rf_dataLength = 0;
    receiveBufferSize = 0;
    receiveDataBufferSize = 0;
    cf_regAdr = startRegister;

    if(writeCommandType == ReqType::SINGLE_DEVICE_WRITE){
        if(device == Device::MULTIPLE_DEVICES){
            status = Status::ERROR;
            return status;
        }
        if(static_cast<uint8_t>(device) >= deviceCount){
            status = Status::ERROR;
            return status;
        }

        cf_devAdr = static_cast<uint8_t>(device);
        for(uint8_t i = 0; i < dataLength; i++)
            writeDataBufferByte(data[i]);
        status = execute(SINGLE_DEVICE_COMMAND_FRAME, 0);
    } else{
        if(device != Device::MULTIPLE_DEVICES){
            status = Status::ERROR;
            return status;
        }
        if(writeCommandType == ReqType::BROADCAST_WRITE_REVERSE){
            if(dataLength != 1){
                status = Status::ERROR;
                return status;
            }
            if(data[0] != static_cast<uint8_t>(Control1::DIR_SEL)){
                status = Status::ERROR;
                return status;
            }
            if(startRegister != static_cast<uint8_t>(ReadWriteRegister::CONTROL1)){
                status = Status::ERROR;
                return status;
            }

            writeDataBufferByte(static_cast<uint8_t>(Control1::DIR_SEL));
        }else if (writeCommandType == ReqType::BROADCAST_WRITE | writeCommandType == ReqType::STACK_WRITE){
            for(uint8_t i = 0; i < dataLength; i++){
                writeDataBufferByte(data[i]);
            }
        }else{
            return Status::ERROR;
        }

        status = execute(MULTI_DEVICE_COMMAND_FRAME, 0);
    }

    return status;
}

Status DaisyChain::readRegisters(ReqType readCommandType, Device device, uint16_t startRegister, uint8_t registerCount) {
    Status status = getCommandInit(readCommandType, 1, &cf_init);
    if(status != Status::COMPLETED){
        return status;
    }

    cf_dataLength = 1;
    rf_dataLength = registerCount;
    cf_regAdr = startRegister;
    writeDataBufferByte(registerCount - 1);

    if(readCommandType == ReqType::SINGLE_DEVICE_READ){
        if(device == Device::MULTIPLE_DEVICES){
            status = Status::ERROR;
            return status;
        }
        if(static_cast<uint8_t>(device) >= deviceCount){
            status = Status::ERROR;
            return status;
        }

        receiveBufferSize = rf_dataLength + TRANSACTION_FRAME_OVERHEAD;
        receiveDataBufferSize = rf_dataLength;
        cf_devAdr = static_cast<uint8_t>(device);

        status = execute(SINGLE_DEVICE_COMMAND_FRAME, 1);
    }else{
        if(device != Device::MULTIPLE_DEVICES){
            status = Status::ERROR;
            return status;
        }

        if(readCommandType == ReqType::STACK_READ){
            receiveBufferSize = (rf_dataLength + TRANSACTION_FRAME_OVERHEAD) * (this->deviceCount - 1);
            receiveDataBufferSize = rf_dataLength * (this->deviceCount - 1);

            status = execute(MULTI_DEVICE_COMMAND_FRAME, this->deviceCount - 1);
        }else if(readCommandType == ReqType::BROADCAST_READ){
            receiveBufferSize = (rf_dataLength + TRANSACTION_FRAME_OVERHEAD) * this->deviceCount;
            receiveDataBufferSize = rf_dataLength * this->deviceCount;

            status = execute(MULTI_DEVICE_COMMAND_FRAME, this->deviceCount);
        }else{
            return Status::ERROR;
        }
    }

    return status;
}

DaisyChain::DaisyChain(UART_HandleTypeDef &uartHandle, TIM_HandleTypeDef &timHandle, uint8_t deviceCount, GPIO::Pin uart_tx, GPIO::Pin uart_rx)
    : uartHandle(&uartHandle), timHandle(&timHandle), deviceCount(deviceCount), UART_TX(uart_tx), UART_RX(uart_rx) {
}

void DaisyChain::init() {
    init_UART_TX_GPIO();
}

Status DaisyChain::ping(Ping_us time_us) {
    HAL_UART_DeInit(uartHandle);

    HAL_GPIO_Init(UART_TX.getPort(), &UART_TX_GPIO_InitStruct);

    if(UART_TX.read() != 1)
        UART_TX.write(GPIO_PinState::GPIO_PIN_SET);

    UART_TX.write(GPIO_PinState::GPIO_PIN_RESET);
    HAL_TIM_Base_Start(timHandle);
    uint16_t startTime = __HAL_TIM_GET_COUNTER(timHandle);
    while(__HAL_TIM_GET_COUNTER(timHandle) - startTime <= static_cast<uint16_t>(time_us));
    UART_TX.write(GPIO_PinState::GPIO_PIN_SET);

    HAL_GPIO_DeInit(UART_TX.getPort(), UART_TX.getPin());

    HAL_UART_Init(uartHandle);

    return Status::COMPLETED;
}

Status DaisyChain::write(ReqType writeCommandType, Device device, ReadWriteRegister startRegister, uint8_t *data, uint8_t dataLength) {
    return writeRegisters(writeCommandType, device, static_cast<uint16_t>(startRegister), data, dataLength);
}

Status DaisyChain::write(ReqType writeCommandType, Device device, OTPRegister startRegister, uint8_t *data, uint8_t dataLength) {
    return writeRegisters(writeCommandType, device, static_cast<uint16_t>(startRegister), data, dataLength);
}

Status DaisyChain::read(ReqType readCommandType, Device device, ReadWriteRegister startRegister, uint8_t registerCount) {
    return readRegisters(readCommandType, device, static_cast<uint16_t>(startRegister), registerCount);
}

Status DaisyChain::read(ReqType readCommandType, Device device, ReadOnlyRegister startRegister, uint8_t registerCount) {
    return readRegisters(readCommandType, device, static_cast<uint16_t>(startRegister), registerCount);
}

Status DaisyChain::readDataBuffer(uint8_t *value) {
    return readDataBufferByte(value);
}

Status DaisyChain::performAutoadressing() {
        uint8_t sendDataAA = 0;
        uint8_t receiveDataAA = 0;

        // send dummy data to sync delay-locked loop
        sendDataAA = static_cast<uint8_t>(OTP_ECC_Test::RESET_VALUE);
        if(write(ReqType::BROADCAST_WRITE, Device::MULTIPLE_DEVICES, ReadWriteRegister::OTP_ECC_TEST, &sendDataAA, 1) != Status::COMPLETED)
            return Status::ERROR;

        // enable autoadressing on all devices
        sendDataAA = static_cast<uint8_t>(Control1::RESET_VALUE) | static_cast<uint8_t>(Control1::ADDR_WR);
        if(write(ReqType::BROADCAST_WRITE, Device::MULTIPLE_DEVICES, ReadWriteRegister::CONTROL1, &sendDataAA, 1) != Status::COMPLETED)
            return Status::ERROR;

        // distribute addresses to all the devices
        for(int i = 0; i < deviceCount; i++){
            sendDataAA = RESET_VALUE_0 | i;
            if(write(ReqType::BROADCAST_WRITE, Device::MULTIPLE_DEVICES, ReadWriteRegister::DIR0_ADDR, &sendDataAA, 1) != Status::COMPLETED)
                return Status::ERROR;
        }

        // set all devices to stack devices
        sendDataAA = static_cast<uint8_t>(Comm_Ctrl::RESET_VALUE) | static_cast<uint8_t>(Comm_Ctrl::STACK_DEV);
        if(write(ReqType::BROADCAST_WRITE, Device::MULTIPLE_DEVICES, ReadWriteRegister::COMM_CTRL, &sendDataAA, 1) != Status::COMPLETED)
            return Status::ERROR;
        sendDataAA = 0;

        // revoke stack device setting for base device
        sendDataAA = static_cast<uint8_t>(Comm_Ctrl::RESET_VALUE);
        if(write(ReqType::SINGLE_DEVICE_WRITE, Device::BASE_DEVICE, ReadWriteRegister::COMM_CTRL, &sendDataAA, 1) != Status::COMPLETED)
            return Status::ERROR;

        // set up top of stack device
        sendDataAA = static_cast<uint8_t>(Comm_Ctrl::RESET_VALUE) | static_cast<uint8_t>(Comm_Ctrl::STACK_DEV) | static_cast<uint8_t>(Comm_Ctrl::TOP_STACK);
        if(write(ReqType::SINGLE_DEVICE_WRITE, static_cast<Device>(deviceCount - 1), ReadWriteRegister::COMM_CTRL, &sendDataAA, 1) != Status::COMPLETED)
            return Status::ERROR;

        // read dummy data to sync delay-locked loop
        if(read(ReqType::BROADCAST_READ, Device::MULTIPLE_DEVICES, ReadWriteRegister::OTP_ECC_TEST, 1) != Status::COMPLETED)
            return Status::ERROR;

        // read back device addresses of all devices and compare them
        if(read(ReqType::BROADCAST_READ, Device::MULTIPLE_DEVICES, ReadWriteRegister::DIR0_ADDR, 1) != Status::COMPLETED)
            return Status::ERROR;
        for(int i = deviceCount - 1; i >= 0; i--){
            if(readDataBuffer(&receiveDataAA) != Status::COMPLETED)
                return Status::ERROR;
            if(receiveDataAA != i)
                return Status::ERROR;
        }

        return Status::COMPLETED;
}

Status DaisyChain::activateVCs(Device device, Active_Cell activeVCConfig) {
        uint8_t sendDataAVC = static_cast<uint8_t>(activeVCConfig);
        if(write(ReqType::SINGLE_DEVICE_WRITE, device, OTPRegister::ACTIVE_CELL, &sendDataAVC, 1) != Status::COMPLETED)
            return Status::ERROR;

        return Status::COMPLETED;
}

Status DaisyChain::readCellVoltages(Device device, float *cellVoltages, uint8_t cellVoltageCount) {
        // configure main adc in continous run mode and start adc
//        uint8_t sendDataRCV = static_cast<uint8_t>(ADC_Ctrl1::MAIN_MODE_CONTINOUS_RUN) | static_cast<uint8_t>(ADC_Ctrl1::MAIN_GO);
//        if(write(ReqType::SINGLE_DEVICE_WRITE, device, ReadWriteRegister::ADC_CTRL1, &sendDataRCV, 1) != Status::COMPLETED)
//            return Status::ERROR;

        // TODO implement exact delay in us
        //Parallel::delay(2);

        // read cell voltages from BQ
        if(read(ReqType::SINGLE_DEVICE_READ, device,  static_cast<ReadOnlyRegister>(static_cast<uint8_t>(ReadOnlyRegister::VCELL1_LO) - (cellVoltageCount * 2 - 1)), cellVoltageCount * 2) != Status::COMPLETED)
            return Status::ERROR;

        // write cell voltages in provided buffer
        uint16_t voltage_raw = 0;
        int16_t voltage_signed = 0;
        uint8_t v_hi = 0;
        uint8_t v_lo = 0;
        for(uint8_t i = 0; i < cellVoltageCount; i++){
            readDataBufferByte(&v_hi);
            readDataBufferByte(&v_lo);

            voltage_raw |= (static_cast<uint16_t>(v_hi) << 8) | static_cast<uint16_t>(v_lo);
            voltage_signed = toSigned(voltage_raw);

            cellVoltages[i] = static_cast<float>(voltage_signed) * MAIN_AUX_ADC_RESOLUTION_VCELL;
        }

        return Status::COMPLETED;
}

// TODO function is not generic in terms of the GPIOs to activate, this is because of the shield NTCs, must be changed later
Status DaisyChain::setActivateGPIOADCs(Device device, uint8_t activeGPIOCount) {
        uint8_t sendDataAGA = static_cast<uint8_t>(GPIO_Conf4::GPIO8_ADC_ONLY) | static_cast<uint8_t>(GPIO_Conf4::GPIO7_ADC_ONLY);
        if(write(ReqType::SINGLE_DEVICE_WRITE, device, OTPRegister::GPIO_CONF4, &sendDataAGA, 1) != Status::COMPLETED)
            return Status::ERROR;

        sendDataAGA = RESET_VALUE_0 | static_cast<uint8_t>(Control2::TSREF_EN);
        if(write(ReqType::SINGLE_DEVICE_WRITE, device, ReadWriteRegister::CONTROL2, &sendDataAGA, 1) != Status::COMPLETED)
            return Status::ERROR;

        return Status::COMPLETED;
}

// TODO function is not generic in terms of the GPIOs to read, this is because of the shield NTCs, must be changed later
// TODO make sure that the wanted gpios are corretly activatated and through an error if not
// TODO write more generic version which can read back cell temperatures
/**
 * This function reads back a the cell temperatures of the wanted device in the chain.
 * The consecutive cells to read the temperatures from has to be connected to the adjacent GPIOs of the called device to ensure correct functionality
 * It is mandatory to call activate the ADCs for the needed GPIOs first.
 *
 * @param device device which should return cell temperatures
 * @param cellTemperatures buffer for the cell temperatures
 * @param cellTemperatureCount number of consecutive cells to read the temperatures from, starting from the first
 * @return status code
 */
Status DaisyChain::readCellTemperature(Device device, float *cellTemperatures, uint8_t cellTemperatureCount) {
//        uint8_t sendDataCT = static_cast<uint8_t>(Control2::TSREF_EN);
//        if(write(ReqType::SINGLE_DEVICE_WRITE, device, ReadWriteRegister::CONTROL2, &sendDataCT, 1) != Status::COMPLETED)
//            return Status::ERROR;

//        sendDataCT = static_cast<uint8_t>(ADC_Ctrl1::RESET_VALUE) | static_cast<uint8_t>(ADC_Ctrl1::MAIN_MODE_CONTINOUS_RUN) | static_cast<uint8_t>(ADC_Ctrl1::MAIN_GO);
//        if(write(ReqType::SINGLE_DEVICE_WRITE, device, ReadWriteRegister::ADC_CTRL1, &sendDataCT, 1) != Status::COMPLETED)
//            return Status::ERROR;

        // TODO implement exact delay in us
        //Parallel::delay(10);

        if(read(ReqType::SINGLE_DEVICE_READ, device,  ReadOnlyRegister::TSREF_HI, 2) != Status::COMPLETED)
            return Status::ERROR;

        uint8_t tsref_hi = 0;
        uint8_t tsref_lo = 0;
        uint16_t tsref_raw = 0;
        float v_tsref = 0;

        readDataBufferByte(&tsref_hi);
        readDataBufferByte(&tsref_lo);

        tsref_raw |= (static_cast<uint16_t>(tsref_hi) << 8) | static_cast<uint16_t>(tsref_lo);
        v_tsref = static_cast<float>(toSigned(tsref_raw)) * MAIN_ADC_RESOLUTION_TSREF;

        // TODO change "static_cast<ReadOnlyRegister>(ReadOnlyRegister::GPIO1_HI + 12)" to "ReadOnlyRegister::GPIO1_HI" to generalize function
        if(read(ReqType::SINGLE_DEVICE_READ, device,  static_cast<ReadOnlyRegister>(static_cast<uint16_t>(ReadOnlyRegister::GPIO1_HI) + 12), cellTemperatureCount * 2) != Status::COMPLETED)
            return Status::ERROR;

        uint8_t gpiox_hi = 0;
        uint8_t gpiox_lo = 0;
        uint16_t gpiox_raw = 0;
        int16_t gpiox_signed = 0;
        float gpiox_voltage  = 0;
        float r_ntc = 0;

        for(uint8_t i = 0; i < cellTemperatureCount; i++){
            readDataBufferByte(&gpiox_hi);
            readDataBufferByte(&gpiox_lo);

            gpiox_raw |= (static_cast<uint16_t>(gpiox_hi) << 8) | static_cast<uint16_t>(gpiox_lo);
            gpiox_signed = toSigned(gpiox_raw);

            gpiox_voltage = static_cast<float>(gpiox_signed) * MAIN_AUX_ADC_RESOLUTION_GPIO;
            r_ntc = ((R_VOLTAGE_DIVIDER * gpiox_voltage) / (v_tsref - gpiox_voltage));
            cellTemperatures[i] = (1 / ((1 / NTC_B) * logf(r_ntc / NTC_R0) + (1 / NTC_T0))) - CELCIUS_KELVIN_OFFSET;
        }

        return Status::COMPLETED;
}

Status DaisyChain::readDieTemperature(float *dieTemperature) {
//        uint8_t sendDataDT = static_cast<uint8_t>(ADC_Ctrl1::RESET_VALUE) | static_cast<uint8_t>(ADC_Ctrl1::MAIN_MODE_CONTINOUS_RUN) | static_cast<uint8_t>(ADC_Ctrl1::MAIN_GO);
//        if(write(ReqType::SINGLE_DEVICE_WRITE, device, ReadWriteRegister::ADC_CTRL1, &sendDataDT, 1) != Status::COMPLETED)
//            return Status::ERROR;

        // TODO implement exact delay in us
        //Parallel::delay(3);

        if(read(ReqType::BROADCAST_READ, Device::MULTIPLE_DEVICES,  ReadOnlyRegister::DIETEMP1_HI, 2) != Status::COMPLETED)
            return Status::ERROR;

        uint8_t dt_hi = 0;
        uint8_t dt_lo = 0;
        uint16_t dt_raw = 0;

        for(int i = deviceCount - 1; i >= 0; i--){
            readDataBufferByte(&dt_hi);
            readDataBufferByte(&dt_lo);

            dt_raw |= (static_cast<uint16_t>(dt_hi) << 8) | static_cast<uint16_t>(dt_lo);
            dieTemperature[i] = static_cast<float>(toSigned(dt_raw)) * MAIN_ADC_RESOLUTION_DIETEMP1;
        }

        return Status::COMPLETED;
}

Status DaisyChain::activateADC(Device device, uint8_t config) {
        uint8_t sendDataAADC = static_cast<uint8_t>(ADC_Ctrl1::RESET_VALUE) | static_cast<uint8_t>(config);
        if(write(ReqType::SINGLE_DEVICE_WRITE, device, ReadWriteRegister::ADC_CTRL1, &sendDataAADC, 1) != Status::COMPLETED)
            return Status::ERROR;

        return Status::COMPLETED;
}

Status DaisyChain::getFaultSummary(Device device, FaultSummary *faults) {
        if(read(ReqType::SINGLE_DEVICE_READ, device,  ReadOnlyRegister::FAULT_SUMMARY, 1) != Status::COMPLETED)
            return Status::ERROR;

        uint8_t dataFS = 0;
        readDataBufferByte(&dataFS);

        uint8_t mask = 1;
        faults->FAULT_PROT = (dataFS >> 7) & mask;
        faults->FAULT_COMP_ADC = (dataFS >> 6) & mask;
        faults->FAULT_OTP = (dataFS >> 5) & mask;
        faults->FAULT_COMM = (dataFS >> 4) & mask;
        faults->FAULT_OTUT = (dataFS >> 3) & mask;
        faults->FAULT_OVUV = (dataFS >> 2) & mask;
        faults->FAULT_SYS = (dataFS >> 1) & mask;
        faults->FAULT_PWR = dataFS & mask;

        return Status::COMPLETED;
}
