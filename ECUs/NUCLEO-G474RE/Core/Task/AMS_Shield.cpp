#include <parallel/Task.hpp>
#include <IO/CAN/generated/CAN1_dbc.h>

#include "../TestModules/BQ79612_Shield/include/BQ79612.h"
#include "../TestModules/BQ79612_Shield/Simulink/BMS.h"
#include "Init.h"
#include "main.h"

using namespace BQ79612;

extern UART_HandleTypeDef huart4;
extern TIM_HandleTypeDef htim16;

[[noreturn]] void runAMS_Shield();

Parallel::NormalTask amsShieldTask(&runAMS_Shield, "AMS-Shield", RTOS::Priority::PriorityNormal);
Parallel::BinaryChannel channelAMSShield(amsShieldTask);

UART::Port &uartPortAMSShield = UART::Port::create<&huart4>();

GPIO::Pin uart_tx = GPIO::Pin(Host_UART_TX_GPIO_Port, Host_UART_TX_Pin);
GPIO::Pin uart_rx = GPIO::Pin(Host_UART_RX_GPIO_Port, Host_UART_RX_Pin);

const uint8_t DEVICE_COUNT = 4;

// Simulink model code
ExtU_BMS_T externalInputs;
ExtY_BMS_T externalOutputs;
BMS bms = BMS();

class AMS_Shield  {
    DaisyChain chain;

  public:
    explicit AMS_Shield() : chain(uartPortAMSShield, channelAMSShield, uart_tx, uart_rx){}

    void generalStartup(){
        chain.init(huart4, htim16, DEVICE_COUNT);

        uartPortAMSShield.setInterruptHandler(UART::InterruptHandler::bind<&BQ79612::DaisyChain::handleInterrupt>(chain));

        chain.wake();

        //Simulink model
//        bms.initialize();

        if(chain.performAutoadressing() != Status::COMPLETED)
            Debug::exception();
    }

    void startUp(){
        uint8_t cfg = static_cast<uint8_t>(ADC_Ctrl1::MAIN_MODE_CONTINOUS_RUN) | static_cast<uint8_t>(ADC_Ctrl1::MAIN_GO);

        chain.activateVCs(Device::STACK_DEVICE2, Active_Cell::_6S);
        chain.activateVCs(Device::STACK_DEVICE3, Active_Cell::_6S);
        Parallel::delay(10);
        chain.setActivateGPIOADCs(Device::STACK_DEVICE2);
        chain.setActivateGPIOADCs(Device::STACK_DEVICE3);
        Parallel::delay(10);
        chain.activateADCs(cfg);
    }

    void readVoltages(){
        float voltages[12] = {};
        if(chain.readCellVoltages(Device::STACK_DEVICE2, &voltages[0], 6) != Status::COMPLETED)
            Debug::exception();
        if(chain.readCellVoltages(Device::STACK_DEVICE3, &voltages[6], 6) != Status::COMPLETED)
            Debug::exception();

//        BSPD_RAW1::TPS_A_REF_VOLTAGE_BSPD.setPhysicalValue(voltages[0]);
//        BSPD_RAW1::TPS_A_VOLTAGE_BSPD.setPhysicalValue(voltages[1]);
//        AMS_CellVoltages1::Voltage_Cell01.setPhysicalValue(0);
//        AMS_CellVoltages1::Voltage_Cell02.setPhysicalValue(var++);
//        AMS_CellVoltages1::Voltage_Cell03.setPhysicalValue(var--);
//        AMS_CellVoltages1::Voltage_Cell04.setPhysicalValue(voltages[3]);
//        AMS_CellVoltages1::Voltage_Cell05.setPhysicalValue(voltages[4]);
//        AMS_CellVoltages1::Voltage_Cell06.setPhysicalValue(voltages[5]);
//        AMS_CellVoltages2::Voltage_Cell07.setPhysicalValue(voltages[6]);
//        AMS_CellVoltages2::Voltage_Cell08.setPhysicalValue(voltages[7]);
//        AMS_CellVoltages2::Voltage_Cell09.setPhysicalValue(voltages[8]);
//        AMS_CellVoltages2::Voltage_Cell10.setPhysicalValue(voltages[9]);
//        AMS_CellVoltages2::Voltage_Cell11.setPhysicalValue(voltages[10]);
//        AMS_CellVoltages2::Voltage_Cell12.setPhysicalValue(voltages[11]);
    }

    void readTemperatures(){
        float temperatures[16];
        Parallel::delay(10);
        if(chain.readCellTemperature(Device::STACK_DEVICE2, &temperatures[0], 8) != Status::COMPLETED)
            Debug::exception();
        if(chain.readCellTemperature(Device::STACK_DEVICE3, &temperatures[8], 8) != Status::COMPLETED)
            Debug::exception();

        float tempMin = temperatures[0];
        float tempMax = temperatures[0];
        for(float value : temperatures){
            if(value > 0){
                if(value < tempMin){
                    tempMin = value;
                }
                if(value > tempMax){
                    tempMax = value;
                }
            }
        }

//                AMS_Simulink::Min_Temperature.setPhysicalValue(tempMin);
//                AMS_Simulink::Max_Temperature.setPhysicalValue(tempMax);

//        AMS_CellTemperatures1::Temperature_Cell01.setPhysicalValue(temperatures[0]);
//        AMS_CellTemperatures1::Temperature_Cell02.setPhysicalValue(temperatures[1]);
//        AMS_CellTemperatures1::Temperature_Cell03.setPhysicalValue(temperatures[2]);
//        AMS_CellTemperatures1::Temperature_Cell04.setPhysicalValue(temperatures[3]);
//        AMS_CellTemperatures1::Temperature_Cell05.setPhysicalValue(temperatures[4]);
//        AMS_CellTemperatures1::Temperature_Cell06.setPhysicalValue(temperatures[5]);
//        AMS_CellTemperatures2::Temperature_Cell07.setPhysicalValue(temperatures[8]);
//        AMS_CellTemperatures2::Temperature_Cell08.setPhysicalValue(temperatures[9]);
//        AMS_CellTemperatures2::Temperature_Cell09.setPhysicalValue(temperatures[10]);
//        AMS_CellTemperatures2::Temperature_Cell10.setPhysicalValue(temperatures[11]);
//        AMS_CellTemperatures2::Temperature_Cell11.setPhysicalValue(temperatures[12]);
//        AMS_CellTemperatures2::Temperature_Cell12.setPhysicalValue(temperatures[13]);
//        AMS_CellTemperatures_Extra::Temperature_Cell_Extra1.setPhysicalValue(temperatures[6]);
//        AMS_CellTemperatures_Extra::Temperature_Cell_Extra2.setPhysicalValue(temperatures[7]);
//        AMS_CellTemperatures_Extra::Temperature_Cell_Extra3.setPhysicalValue(temperatures[14]);
//        AMS_CellTemperatures_Extra::Temperature_Cell_Extra4.setPhysicalValue(temperatures[15]);
    }

    void readDieTemperatures(){
        float temperatures[4];
        Parallel::delay(1);
        if(chain.readDieTemperature(temperatures) != Status::COMPLETED)
            Debug::exception();

//        AMS_DieTemperatures::Temperature_Die_BD.setPhysicalValue(temperatures[0]);
//        AMS_DieTemperatures::Temperature_Die_S1_D1.setPhysicalValue(temperatures[2]);
//        AMS_DieTemperatures::Temperature_Die_S1_D2.setPhysicalValue(temperatures[3]);
    }

    void readFaults(){
        FaultSummary faults_BD;
        FaultSummary faults_S1_D1;
        FaultSummary faults_S1_D2;

        if(chain.getFaultSummary(Device::BASE_DEVICE, &faults_BD) != Status::COMPLETED)
            Debug::exception();
        if(chain.getFaultSummary(Device::STACK_DEVICE2, &faults_S1_D1) != Status::COMPLETED)
            Debug::exception();
        if(chain.getFaultSummary(Device::STACK_DEVICE3, &faults_S1_D2) != Status::COMPLETED)
            Debug::exception();
    }
};

void runAMS_Shield(){

    AMS_Shield amsShield;

    amsShield.generalStartup();
    amsShield.startUp();

    for(;;){
        // Diagnosis functionality
        amsShield.readDieTemperatures();
        amsShield.readFaults();

        // Main functionality
        amsShield.readTemperatures();
        amsShield.readVoltages();
        Parallel::delay(1);
    }
}

void initAMS_Shield(){
    amsShieldTask.init();
}