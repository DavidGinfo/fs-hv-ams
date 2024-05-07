// Stand 01.03. -> alles hat funktioniert



//#include <parallel/Task.hpp>
//#include <IO/CAN/generated/CAN1_dbc.h>
//
//#include "../TestModules/BQ79612_Shield/include/BQ79612.h"
//#include "Init.h"
//#include "main.h"
//
//using namespace BQ79612;
//
//extern UART_HandleTypeDef huart4;
//extern TIM_HandleTypeDef htim16;
//
//[[noreturn]] void runAMS_Shield();
//
//Parallel::NormalTask amsShieldTask(&runAMS_Shield, "AMS-Shield");
//
//GPIO::Pin uart_tx = GPIO::Pin(Host_UART_TX_GPIO_Port, Host_UART_TX_Pin);
//GPIO::Pin uart_rx = GPIO::Pin(Host_UART_RX_GPIO_Port, Host_UART_RX_Pin);
//
////uint8_t devices = 3;
//
////DaisyChain ams(huart4, htim16, devices, uart_tx, uart_rx);
//
//class AMS_Shield  {
//    DaisyChain chain;
//
//  public:
//    explicit AMS_Shield(uint8_t devices) : chain(huart4, htim16, devices, uart_tx, uart_rx){}
//
//    void generalStartup(){
//        chain.init();
//
//        chain.ping(Ping_us::HARDWARE_RESET);
//        Parallel::delay(100);
//        chain.ping(Ping_us::WAKE);
//        // wait min 10ms + 600us per device wake propagation
//        Parallel::delay(100);
//        chain.ping(Ping_us::WAKE);
//        // wait min 10ms + 600us per device wake propagation, dont know why have to to this twice
//        Parallel::delay(100);
//
//        if(chain.performAutoadressing() != Status::COMPLETED)
//            Debug::exception();
//    }
//
//    void startUp(){
//        uint8_t cfg = static_cast<uint8_t>(ADC_Ctrl1::MAIN_MODE_CONTINOUS_RUN) | static_cast<uint8_t>(ADC_Ctrl1::MAIN_GO);
//        Parallel::delay(10);
//
//        chain.activateVCs(Device::STACK_DEVICE2, Active_Cell::_6S);
//
//        chain.setActivateGPIOADCs(Device::STACK_DEVICE2, 2);
//
//        chain.activateADCs(cfg);
//    }
//
//    void readVoltages(){
//        float voltages[2] = {};
//        if(chain.readCellVoltages(Device::STACK_DEVICE2, voltages, 2) != Status::COMPLETED)
//            Debug::exception();
//
//        AMS_Shield_CellVoltages::Voltage_Cell1.setPhysicalValue(voltages[0]);
//        AMS_Shield_CellVoltages::Voltage_Cell2.setPhysicalValue(voltages[1]);
//    }
//
//    void readTemperatures(){
//        float temperatures[2];
//        if(chain.readCellTemperature(Device::STACK_DEVICE2, temperatures, 2) != Status::COMPLETED)
//            Debug::exception();
//
//        AMS_Shield_CellTemperatures::Temperature_Cell1.setPhysicalValue(temperatures[0]);
//        AMS_Shield_CellTemperatures::Temperature_Cell2.setPhysicalValue(temperatures[1]);
//    }
//
//    void readDieTemperatures(){
//        float temperatures[3];
//        if(chain.readDieTemperature(temperatures) != Status::COMPLETED)
//            Debug::exception();
//
//        AMS_Shield_DieTemperatures::Temperature_Die_BaseDevice.setPhysicalValue(temperatures[0]);
//        AMS_Shield_DieTemperatures::Temperature_Die_StackDevice1.setPhysicalValue(temperatures[1]);
//        AMS_Shield_DieTemperatures::Temperature_Die_StackDevice2.setPhysicalValue(temperatures[2]);
//    }
//
//    void readFaults(){
//        FaultSummary faults;
//
//        if(chain.getFaultSummary(Device::BASE_DEVICE, &faults) != Status::COMPLETED)
//            Debug::exception();
//
//        AMS_Faults1::Fault_Protection.setPhysicalValue(faults.faultProt);
//        AMS_Faults1::Fault_ADC_Comparison.setPhysicalValue(faults.faultCompADC);
//        AMS_Faults1::Fault_OTP.setPhysicalValue(faults.faultOTP);
//        AMS_Faults1::Fault_Communication.setPhysicalValue(faults.faultComm);
//        AMS_Faults1::Fault_OTUT.setPhysicalValue(faults.faultOTUT);
//        AMS_Faults1::Fault_OVOV.setPhysicalValue(faults.faultOVUV);
//        AMS_Faults1::Fault_System.setPhysicalValue(faults.faultSys);
//        AMS_Faults1::Fault_Power_Rail.setPhysicalValue(faults.faultPwr);
//    }
//};
//
//
//// TODO wenn BQ warm wieder passt alles außer man schaltet ihn aus, dann lässt er sich zwar aufwecken, aber antwortet erst wieder auf Nachrichten wenn er bei ca 33 grad angekommen ist
//void runAMS_Shield(){
////    if(ams.activateVCs(Device::STACK_DEVICE2, Active_Cell::_6S) != Status::COMPLETED)
////        status = false;
//
////    float voltages[2] = {};
////    if(ams.readCellVoltages(Device::STACK_DEVICE2, voltages, 2) != Status::COMPLETED)
////        status = false;
////
////    if(ams.setActivateGPIOADCs(Device::STACK_DEVICE2, 2) != Status::COMPLETED)
////        status = false;
//
////    float cellTemperatures[2] = {};
////    float dieTemperatures[3] = {};
//
//    AMS_Shield amsShield(3);
//
//    amsShield.generalStartup();
//    amsShield.startUp();
//
//    for(;;){
////        if(ams.readCellTemperature(Device::STACK_DEVICE2, cellTemperatures, 2) != Status::COMPLETED)
////            status = false;
////        AMS_Shield_CellTemperatures::Temperature_Cell1.setPhysicalValue(cellTemperatures[0]);
////        AMS_Shield_CellTemperatures::Temperature_Cell2.setPhysicalValue(cellTemperatures[1]);
//
////        if(ams.readDieTemperature(Device::BASE_DEVICE, &dieTemperatures[0]) != Status::COMPLETED)
////            status = false;
////        AMS_Shield_DieTemperatures::Temperature_Die_BaseDevice.setPhysicalValue(dieTemperatures[0]);
////
////        if(ams.readDieTemperature(Device::STACK_DEVICE1, &dieTemperatures[1]) != Status::COMPLETED)
////            status = false;
////        AMS_Shield_DieTemperatures::Temperature_Die_StackDevice1.setPhysicalValue(dieTemperatures[1]);
////
////        if(ams.readDieTemperature(Device::STACK_DEVICE2, &dieTemperatures[2]) != Status::COMPLETED)
////            status = false;
////        AMS_Shield_DieTemperatures::Temperature_Die_StackDevice2.setPhysicalValue(dieTemperatures[2]);
//
//        amsShield.readTemperatures();
//        amsShield.readVoltages();
//        amsShield.readDieTemperatures();
//        amsShield.readFaults();
//
//        Parallel::delay(1);
//    }
//}
//
//void initAMS_Shield(){
//    amsShieldTask.init();
//}

//void runAMS_Shield(){
//    bool status = true;
//
//    ams.init();
//
//    ams.ping(Ping_us::SHUTDOWN);
//    ams.ping(Ping_us::WAKE);
//    Parallel::delay(100);
//    ams.ping(Ping_us::WAKE);
//    // wait min 10ms + 600us per device wake propagation
//    Parallel::delay(100);
//
//    // hardcoded autoadressing
//
//    uint8_t buffer6_1[6] = {0xD0, 0x03, 0x4C, 0x00, 0xFC, 0x24}; // D0 03 4C 00 FC 24
//    if(HAL_UART_Transmit(&huart4, buffer6_1, 6, 10) != HAL_OK)
//        status = false;
//
//    uint8_t buffer6_2[6] = {0xD0, 0x03, 0x09, 0x01, 0x0F, 0x74}; // D0 03 09 01 0F 74
//    if(HAL_UART_Transmit(&huart4, buffer6_2, 6, 100) != HAL_OK)
//        status = false;
//
//    uint8_t buffer6_3[6] = {0xD0, 0x03, 0x06, 0x00, 0xCB, 0x44}; // D0 03 06 00 CB 44
//    if(HAL_UART_Transmit(&huart4, buffer6_3, 6, 10) != HAL_OK)
//        status = false;
//
//    uint8_t buffer6_4[6] = {0xD0, 0x03, 0x06, 0x01, 0x0A, 0x84}; // D0 03 06 01 0A 84
//    if(HAL_UART_Transmit(&huart4, buffer6_4, 6, 10) != HAL_OK)
//        status = false;
//
//    uint8_t buffer6_5[6] = {0xD0, 0x03, 0x06, 0x02, 0x4A, 0x85}; // D0 03 06 02 4A 85
//    if(HAL_UART_Transmit(&huart4, buffer6_5, 6, 10) != HAL_OK)
//        status = false;
//
//    uint8_t buffer6_6[6] = {0xD0, 0x03, 0x08, 0x02, 0x4E, 0xE5}; // D0 03 08 02 4E E5
//    if(HAL_UART_Transmit(&huart4, buffer6_6, 6, 10) != HAL_OK)
//        status = false;
//
//    uint8_t buffer7_7[7] = {0x90, 0x00, 0x03, 0x08, 0x00, 0x13, 0xDD}; // 90 00 03 08 00 13 DD
//    if(HAL_UART_Transmit(&huart4, buffer7_7, 7, 10) != HAL_OK)
//        status = false;
//
//    uint8_t buffer7_8[7] = {0x90, 0x02, 0x03, 0x08, 0x03, 0x52, 0x64}; // 90 02 03 08 03 52 64
//    if(HAL_UART_Transmit(&huart4, buffer7_8, 7, 10) != HAL_OK)
//        status = false;
//
//    // single stack1 device read part id -> does not work
//    uint8_t bufferexps1[7] = {0x80, 0x01, 0x05, 0x00, 0x00, 0x34, 0x23}; // 80 01 05 00 00 34 23
//    if(HAL_UART_Transmit(&huart4, bufferexps1, 7, 10) != HAL_OK)
//        status = false;
//    uint8_t bufferexprs1[7] = {};
//    if(HAL_UART_Receive(&huart4, bufferexprs1, 7, 10) != HAL_OK)
//        status = false;
//
//    uint8_t buffersr[6] = {0xA0, 0x03, 0x06, 0x00, 0xD1, 0x84}; // A0 03 06 00 D1 84
//    if(HAL_UART_Transmit(&huart4, buffersr, 6, 10) != HAL_OK)
//        status = false;
//    uint8_t buffersre[14] = {};
//    if(HAL_UART_Receive(&huart4, buffersre, 14, 100) != HAL_OK)
//        status = false;
//
//    for(;;){
//        Parallel::delay(10);
//    }
//}