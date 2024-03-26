#include "../include/BQ79612_Manager.h"

using namespace BQ79612;

Status Manager::performAutoadressing() {
    uint8_t sendDataAA = 0;
    uint8_t receiveDataAA = 0;

    // send dummy data to sync delay-locked loop
    sendDataAA = static_cast<uint8_t>(OTP_ECC_Test::RESET_VALUE);
    if(daisyChain.write(ReqType::BROADCAST_WRITE, Device::MULTIPLE_DEVICES, ReadWriteRegister::OTP_ECC_TEST, &sendDataAA, 1) != Status::COMPLETED)
        return Status::ERROR;

    // enable autoadressing on all devices
    sendDataAA = static_cast<uint8_t>(Control1::RESET_VALUE) | static_cast<uint8_t>(Control1::ADDR_WR);
    if(daisyChain.write(ReqType::BROADCAST_WRITE, Device::MULTIPLE_DEVICES, ReadWriteRegister::CONTROL1, &sendDataAA, 1) != Status::COMPLETED)
        return Status::ERROR;

    // distribute addresses to all the devices
    for(int i = 0; i < daisyChain.deviceCount; i++){
        sendDataAA = RESET_VALUE_0 | i;
        if(daisyChain.write(ReqType::BROADCAST_WRITE, Device::MULTIPLE_DEVICES, ReadWriteRegister::DIR0_ADDR, &sendDataAA, 1) != Status::COMPLETED)
            return Status::ERROR;
    }

    // set all devices to stack devices
    sendDataAA = static_cast<uint8_t>(Comm_Ctrl::RESET_VALUE) | static_cast<uint8_t>(Comm_Ctrl::STACK_DEV);
    if(daisyChain.write(ReqType::BROADCAST_WRITE, Device::MULTIPLE_DEVICES, ReadWriteRegister::COMM_CTRL, &sendDataAA, 1) != Status::COMPLETED)
        return Status::ERROR;
    sendDataAA = 0;

    // revoke stack device setting for base device
    sendDataAA = static_cast<uint8_t>(Comm_Ctrl::RESET_VALUE);
    if(daisyChain.write(ReqType::SINGLE_DEVICE_WRITE, Device::BASE_DEVICE, ReadWriteRegister::COMM_CTRL, &sendDataAA, 1) != Status::COMPLETED)
        return Status::ERROR;

    // set up top of stack device
    sendDataAA = static_cast<uint8_t>(Comm_Ctrl::RESET_VALUE) | static_cast<uint8_t>(Comm_Ctrl::STACK_DEV) | static_cast<uint8_t>(Comm_Ctrl::TOP_STACK);
    if(daisyChain.write(ReqType::SINGLE_DEVICE_WRITE, static_cast<Device>(daisyChain.deviceCount - 1), ReadWriteRegister::COMM_CTRL, &sendDataAA, 1) != Status::COMPLETED)
        return Status::ERROR;

    // read dummy data to sync delay-locked loop
    if(daisyChain.read(ReqType::BROADCAST_READ, Device::MULTIPLE_DEVICES, ReadWriteRegister::OTP_ECC_TEST, 1) != Status::COMPLETED)
        return Status::ERROR;

    // read back device addresses of all devices and compare them
    if(daisyChain.read(ReqType::BROADCAST_READ, Device::MULTIPLE_DEVICES, ReadWriteRegister::DIR0_ADDR, 1) != Status::COMPLETED)
        return Status::ERROR;
    for(int i = daisyChain.deviceCount - 1; i >= 0; i--){
        if(daisyChain.readDataBuffer(&receiveDataAA) != Status::COMPLETED)
            return Status::ERROR;
        if(receiveDataAA != i)
            return Status::ERROR;
    }

    return Status::COMPLETED;
}

Status Manager::activateVCs(Device device, Active_Cell activeVCConfig) {
    uint8_t sendDataAVC = static_cast<uint8_t>(activeVCConfig);
    if(daisyChain.write(ReqType::SINGLE_DEVICE_WRITE, device, OTPRegister::ACTIVE_CELL, &sendDataAVC, 1) != Status::COMPLETED)
        return Status::ERROR;

    return Status::COMPLETED;
}

Status Manager::readCellVoltages(Device device, float *cellVoltages, uint8_t cellVoltageCount) {
    // configure main adc in continous run mode and start adc
    uint8_t sendDataRCV = static_cast<uint8_t>(ADC_Ctrl1::MAIN_MODE_CONTINOUS_RUN) | static_cast<uint8_t>(ADC_Ctrl1::MAIN_GO);
    if(daisyChain.write(ReqType::SINGLE_DEVICE_WRITE, device, ReadWriteRegister::ADC_CTRL1, &sendDataRCV, 1) != Status::COMPLETED)
        return Status::ERROR;

    // TODO implement exact delay in us
    Parallel::delay(15);

    // read cell voltages from BQ
    if(daisyChain.read(ReqType::SINGLE_DEVICE_READ, device,  static_cast<ReadOnlyRegister>(static_cast<uint16_t>(ReadOnlyRegister::VCELL1_LO) - (cellVoltageCount * 2 - 1)), cellVoltageCount * 2) != Status::COMPLETED)
        return Status::ERROR;

    // write cell voltages in provided buffer
    // TODO werte auf 0 zurücksetzen
    uint16_t voltage_raw = 0;
    int16_t voltage_signed = 0;
    uint8_t v_hi = 0;
    uint8_t v_lo = 0;
    for(int8_t i = cellVoltageCount - 1; i >= 0; i--){
        daisyChain.readDataBuffer(&v_hi);
        daisyChain.readDataBuffer(&v_lo);

        voltage_raw |= (static_cast<uint16_t>(v_hi) << 8) | static_cast<uint16_t>(v_lo);
        voltage_signed = toSigned(voltage_raw);

        cellVoltages[i] = static_cast<float>(voltage_signed) * MAIN_AUX_ADC_RESOLUTION_VCELL / FACTOR_MICRO;
    }

    return Status::COMPLETED;
}

// TODO function is not generic in terms of the GPIOs to activate, this is because of the shield NTCs, must be changed later
Status Manager::setActivateGPIOADCs(Device device, uint8_t activeGPIOCount) {
    uint8_t sendDataAGA = static_cast<uint8_t>(GPIO_Conf4::GPIO8_ADC_ONLY) | static_cast<uint8_t>(GPIO_Conf4::GPIO7_ADC_ONLY);
    if(daisyChain.write(ReqType::SINGLE_DEVICE_WRITE, device, OTPRegister::GPIO_CONF4, &sendDataAGA, 1) != Status::COMPLETED)
        return Status::ERROR;

    sendDataAGA = RESET_VALUE_0 | static_cast<uint8_t>(Control2::TSREF_EN);
    if(daisyChain.write(ReqType::SINGLE_DEVICE_WRITE, device, ReadWriteRegister::CONTROL2, &sendDataAGA, 1) != Status::COMPLETED)
        return Status::ERROR;

    Parallel::delay(2);

    return Status::COMPLETED;
}

// TODO function is not generic in terms of the GPIOs to read, this is because of the shield NTCs, must be changed later
// TODO make sure that the wanted gpios are corretly activatated and through an error if not, also check ADC config
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
Status Manager::readCellTemperature(Device device, float *cellTemperatures, uint8_t cellTemperatureCount) {
    if(daisyChain.read(ReqType::SINGLE_DEVICE_READ, device,  ReadOnlyRegister::TSREF_HI, 2) != Status::COMPLETED)
        return Status::ERROR;

    uint8_t tsref_hi = 0;
    uint8_t tsref_lo = 0;
    uint16_t tsref_raw = 0;
    float v_tsref = 0;

    daisyChain.readDataBuffer(&tsref_hi);
    daisyChain.readDataBuffer(&tsref_lo);

    tsref_raw |= (static_cast<uint16_t>(tsref_hi) << 8) | static_cast<uint16_t>(tsref_lo);
    v_tsref = static_cast<float>(toSigned(tsref_raw)) * MAIN_ADC_RESOLUTION_TSREF;

    Parallel::delay(2);

    // TODO change "static_cast<ReadOnlyRegister>(ReadOnlyRegister::GPIO1_HI + 12)" to "ReadOnlyRegister::GPIO1_HI" to generalize function
    if(daisyChain.read(ReqType::SINGLE_DEVICE_READ, device,  static_cast<ReadOnlyRegister>(static_cast<uint16_t>(ReadOnlyRegister::GPIO1_HI) + 12), cellTemperatureCount * 2) != Status::COMPLETED)
        return Status::ERROR;

    uint8_t gpiox_hi = 0;
    uint8_t gpiox_lo = 0;
    uint16_t gpiox_raw = 0;
    int16_t gpiox_signed = 0;
    float gpiox_voltage  = 0;
    float r_ntc = 0;

    // TODO werte zurücksetzen auf 0
    for(uint8_t i = 0; i < cellTemperatureCount; i++){
        daisyChain.readDataBuffer(&gpiox_hi);
        daisyChain.readDataBuffer(&gpiox_lo);

        gpiox_raw |= (static_cast<uint16_t>(gpiox_hi) << 8) | static_cast<uint16_t>(gpiox_lo);
        gpiox_signed = toSigned(gpiox_raw);

        gpiox_voltage = static_cast<float>(gpiox_signed) * MAIN_AUX_ADC_RESOLUTION_GPIO;
        r_ntc = ((R_VOLTAGE_DIVIDER * gpiox_voltage) / (v_tsref - gpiox_voltage));
        cellTemperatures[i] = (1 / ((1 / NTC_B) * logf(r_ntc / NTC_R0) + (1 / NTC_T0))) - CELCIUS_KELVIN_OFFSET;
    }

    return Status::COMPLETED;
}

Status Manager::readDieTemperature(float *dieTemperature) {
    if(daisyChain.read(ReqType::BROADCAST_READ, Device::MULTIPLE_DEVICES,  ReadOnlyRegister::DIETEMP1_HI, 2) != Status::COMPLETED)
        return Status::ERROR;

    uint8_t dt_hi = 0;
    uint8_t dt_lo = 0;
    uint16_t dt_raw = 0;

    for(int i = daisyChain.deviceCount - 1; i >= 0; i--){
        daisyChain.readDataBuffer(&dt_hi);
        daisyChain.readDataBuffer(&dt_lo);

        dt_raw = 0;
        dt_raw |= (static_cast<uint16_t>(dt_hi) << 8) | static_cast<uint16_t>(dt_lo);
        dieTemperature[i] = static_cast<float>(toSigned(dt_raw)) * MAIN_ADC_RESOLUTION_DIETEMP1;
    }

    return Status::COMPLETED;
}

// TODO check if GPIO and VC configuration already took place
// more generic function which could config specific devices
/**
 * Must be called and only called after VC and GPIOs are configured
 *
 * @param config
 * @return
 */
Status Manager::activateADCs(uint8_t config) {
    return daisyChain.write(ReqType::BROADCAST_WRITE, Device::MULTIPLE_DEVICES, ReadWriteRegister::ADC_CTRL1, &config, 1);
}

Status Manager::getFaultSummary(Device device, FaultSummary *faults) {
    if(daisyChain.read(ReqType::SINGLE_DEVICE_READ, device,  ReadOnlyRegister::FAULT_SUMMARY, 1) != Status::COMPLETED)
        return Status::ERROR;

    uint8_t dataFS = 0;
    daisyChain.readDataBuffer(&dataFS);

    uint8_t mask = 1;
    faults->faultProt      = (dataFS >> 7) & mask;
    faults->faultCompADC   = (dataFS >> 6) & mask;
    faults->faultOTP       = (dataFS >> 5) & mask;
    faults->faultComm      = (dataFS >> 4) & mask;
    faults->faultOTUT      = (dataFS >> 3) & mask;
    faults->faultOVUV      = (dataFS >> 2) & mask;
    faults->faultSys       = (dataFS >> 1) & mask;
    faults->faultPwr       = dataFS & mask;

    return Status::COMPLETED;
}

void Manager::wake() {
    this->daisyChain.init();

    this->daisyChain.ping(Ping_us::HARDWARE_RESET);
    Parallel::delay(100);
    this->daisyChain.ping(Ping_us::WAKE);
    // wait min 10ms + 600us per device wake propagation
    Parallel::delay(100);
    this->daisyChain.ping(Ping_us::WAKE);
    // wait min 10ms + 600us per device wake propagation, dont know why have to to this twice
    Parallel::delay(100);
}
Manager::Manager(DaisyChain daisyChain) : daisyChain(daisyChain){
}
//Manager::Manager(UART_HandleTypeDef &uartHandle, TIM_HandleTypeDef &timHandle, uint8_t deviceCount, GPIO::Pin uart_tx, GPIO::Pin uart_rx)
  //  : daisyChain(uartHandle, timHandle, deviceCount, uart_tx, uart_rx){}
