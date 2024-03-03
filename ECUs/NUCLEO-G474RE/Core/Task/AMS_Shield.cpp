#include <parallel/Task.hpp>
#include <IO/CAN/generated/CAN1_dbc.h>

#include "../TestModules/BQ79612_Shield/include/BQ79612.h"
#include "../TestModules/BQ79612_Shield/include/BQ79612_Manager.h"
#include "../TestModules/BQ79612_Shield/Simulink//BMS.h"
#include "Init.h"
#include "main.h"

using namespace BQ79612;

extern UART_HandleTypeDef huart4;
extern TIM_HandleTypeDef htim16;

[[noreturn]] void runAMS_Shield();

Parallel::NormalTask amsShieldTask(&runAMS_Shield, "AMS-Shield");

GPIO::Pin uart_tx = GPIO::Pin(Host_UART_TX_GPIO_Port, Host_UART_TX_Pin);
GPIO::Pin uart_rx = GPIO::Pin(Host_UART_RX_GPIO_Port, Host_UART_RX_Pin);

const uint8_t DEVICE_COUNT = 3;

DaisyChain daisyChain = DaisyChain(huart4, htim16, DEVICE_COUNT, uart_tx, uart_rx);

// Simulink model code
ExtU_BMS_T externalInputs;
ExtY_BMS_T externalOutputs;
BMS bms = BMS();

class AMS_Shield  {
    Manager amsManager;

  public:
    AMS_Shield(DaisyChain dChain) : amsManager(dChain){}

    void generalStartup(){
        amsManager.wake();

        if(amsManager.performAutoadressing() != Status::COMPLETED)
            Debug::exception();
    }

    void startShield(){
        uint8_t cfg = static_cast<uint8_t>(ADC_Ctrl1::MAIN_MODE_CONTINOUS_RUN) | static_cast<uint8_t>(ADC_Ctrl1::MAIN_GO);
        Parallel::delay(10);

        amsManager.activateVCs(Device::STACK_DEVICE2, Active_Cell::_6S);

        amsManager.setActivateGPIOADCs(Device::STACK_DEVICE2, 2);

        amsManager.activateADCs(cfg);
    }

    void readVoltages(){
        float voltages[2] = {};
        if(amsManager.readCellVoltages(Device::STACK_DEVICE2, voltages, 2) != Status::COMPLETED)
            Debug::exception();

        AMS_Shield_CellVoltages::Voltage_Cell1.setPhysicalValue(voltages[0]);
        AMS_Shield_CellVoltages::Voltage_Cell2.setPhysicalValue(voltages[1]);

        //Simulink model
        externalInputs.BMSVoltageInData_c.Voltage1 = voltages[0];
        externalInputs.BMSVoltageInData_c.Voltage2 = voltages[1];
        externalInputs.BMSVoltageInData_c.Voltage3 = 3.5;
        externalInputs.BMSVoltageInData_c.Voltage4 = 3.5;
        externalInputs.BMSVoltageInData_c.Voltage5 = 3.5;
        externalInputs.BMSVoltageInData_c.Voltage6 = 3.5;
        externalInputs.BMSVoltageInData_c.Voltage7 = 3.5;
        externalInputs.BMSVoltageInData_c.Voltage8 = 3.5;
        externalInputs.BMSVoltageInData_c.Voltage9 = 3.5;
        externalInputs.BMSVoltageInData_c.Voltage10 = 3.5;
        externalInputs.BMSVoltageInData_c.Voltage11 = 3.5;
        externalInputs.BMSVoltageInData_c.Voltage12 = 3.5;
    }

    void readTemperatures(){
        float temperatures[2];
        if(amsManager.readCellTemperature(Device::STACK_DEVICE2, temperatures, 2) != Status::COMPLETED)
            Debug::exception();

        AMS_Shield_CellTemperatures::Temperature_Cell1.setPhysicalValue(temperatures[0]);
        AMS_Shield_CellTemperatures::Temperature_Cell2.setPhysicalValue(temperatures[1]);

        //Simulink model
        externalInputs.BMSTempInData_f.Temp1 = temperatures[0];
        externalInputs.BMSTempInData_f.Temp2 = temperatures[1];
        externalInputs.BMSTempInData_f.Temp3 = 25;
        externalInputs.BMSTempInData_f.Temp4 = 25;
        externalInputs.BMSTempInData_f.Temp5 = 25;
        externalInputs.BMSTempInData_f.Temp6 = 25;
        externalInputs.BMSTempInData_f.Temp7 = 25;
        externalInputs.BMSTempInData_f.Temp8 = 25;
        externalInputs.BMSTempInData_f.Temp9 = 25;
        externalInputs.BMSTempInData_f.Temp10 = 25;
        externalInputs.BMSTempInData_f.Temp11 = 25;
        externalInputs.BMSTempInData_f.Temp12 = 25;
    }

    void readDieTemperatures(){
        float temperatures[3];
        if(amsManager.readDieTemperature(temperatures) != Status::COMPLETED)
            Debug::exception();

        AMS_Shield_DieTemperatures::Temperature_Die_BaseDevice.setPhysicalValue(temperatures[0]);
        AMS_Shield_DieTemperatures::Temperature_Die_StackDevice1.setPhysicalValue(temperatures[1]);
        AMS_Shield_DieTemperatures::Temperature_Die_StackDevice2.setPhysicalValue(temperatures[2]);
    }

    void readFaults(){
        FaultSummary faults;

        if(amsManager.getFaultSummary(Device::BASE_DEVICE, &faults) != Status::COMPLETED)
            Debug::exception();

        AMS_Shield_Fault_Summary::Fault_Protection.setPhysicalValue(faults.faultProt);
        AMS_Shield_Fault_Summary::Fault_ADC_Comparison.setPhysicalValue(faults.faultCompADC);
        AMS_Shield_Fault_Summary::Fault_OTP.setPhysicalValue(faults.faultOTP);
        AMS_Shield_Fault_Summary::Fault_Communication.setPhysicalValue(faults.faultComm);
        AMS_Shield_Fault_Summary::Fault_OTUT.setPhysicalValue(faults.faultOTUT);
        AMS_Shield_Fault_Summary::Fault_OVOV.setPhysicalValue(faults.faultOVUV);
        AMS_Shield_Fault_Summary::Fault_System.setPhysicalValue(faults.faultSys);
        AMS_Shield_Fault_Summary::Fault_Power_Rail.setPhysicalValue(faults.faultPwr);
    }
};

void runAMS_Shield(){

    AMS_Shield amsShield(daisyChain);

    amsShield.generalStartup();
    amsShield.startShield();

    //Simulink model
    bms.initialize();

    for(;;){
        amsShield.readTemperatures();
        amsShield.readVoltages();
        amsShield.readDieTemperatures();
        amsShield.readFaults();

        //Simulink model
        externalInputs.BatteryState = BatteryMode::Driving;
        externalInputs.BMSCurrentInData = 50;
        bms.setExternalInputs(&externalInputs);
        bms.step();
        externalOutputs = bms.getExternalOutputs();
        AMS_Shield_Simulink::Simulink_Voltage.setPhysicalValue(externalOutputs.BMSActualValues.Voltage);
        AMS_Shield_Simulink::Simulink_BatTemp.setPhysicalValue(externalOutputs.BMSActualValues.BatTemp);
        AMS_Shield_Simulink::Simulink_BatCurrent.setPhysicalValue(externalOutputs.BMSActualValues.BatCurrent);
        AMS_Shield_Simulink::Simulink_BatSOC.setPhysicalValue(externalOutputs.BMSActualValues.BatSOC);

        Parallel::delay(1);
    }
}

void initAMS_Shield(){
    amsShieldTask.init();
}