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

const uint8_t DEVICE_COUNT = 4;

DaisyChain daisyChain = DaisyChain(huart4, htim16, DEVICE_COUNT, uart_tx, uart_rx);

// Simulink model code
ExtU_BMS_T externalInputs;
ExtY_BMS_T externalOutputs;
BMS bms = BMS();

class AMS_Shield  {
    Manager amsManager;

  public:
    AMS_Shield(DaisyChain dChain) : amsManager(dChain){}
    //explicit AMS_Shield() : amsManager(huart4, htim16, DEVICE_COUNT, uart_tx, uart_rx){}


    void generalStartup(){
        amsManager.wake();

        //Simulink model
        bms.initialize();

        if(amsManager.performAutoadressing() != Status::COMPLETED)
            Debug::exception();
    }

    void startUp(){
        uint8_t cfg = static_cast<uint8_t>(ADC_Ctrl1::MAIN_MODE_CONTINOUS_RUN) | static_cast<uint8_t>(ADC_Ctrl1::MAIN_GO);
        Parallel::delay(10);

        amsManager.activateVCs(Device::STACK_DEVICE2, Active_Cell::_6S);
        Parallel::delay(10);
        amsManager.activateVCs(Device::STACK_DEVICE3, Active_Cell::_6S);

        amsManager.setActivateGPIOADCs(Device::STACK_DEVICE2);
        Parallel::delay(10);
        amsManager.setActivateGPIOADCs(Device::STACK_DEVICE3);

        amsManager.activateADCs(cfg);
    }

    void readVoltages(){
        float voltages[12] = {};
        if(amsManager.readCellVoltages(Device::STACK_DEVICE2, &voltages[0], 6) != Status::COMPLETED)
            Debug::exception();
        if(amsManager.readCellVoltages(Device::STACK_DEVICE3, &voltages[6], 6) != Status::COMPLETED)
            Debug::exception();

        AMS_CellVoltages1::Voltage_Cell01.setPhysicalValue(voltages[0]);
        AMS_CellVoltages1::Voltage_Cell02.setPhysicalValue(voltages[1]);
        AMS_CellVoltages1::Voltage_Cell03.setPhysicalValue(voltages[2]);
        AMS_CellVoltages1::Voltage_Cell04.setPhysicalValue(voltages[3]);
        AMS_CellVoltages1::Voltage_Cell05.setPhysicalValue(voltages[4]);
        AMS_CellVoltages1::Voltage_Cell06.setPhysicalValue(voltages[5]);
        AMS_CellVoltages2::Voltage_Cell07.setPhysicalValue(voltages[6]);
        AMS_CellVoltages2::Voltage_Cell08.setPhysicalValue(voltages[7]);
        AMS_CellVoltages2::Voltage_Cell09.setPhysicalValue(voltages[8]);
        AMS_CellVoltages2::Voltage_Cell10.setPhysicalValue(voltages[9]);
        AMS_CellVoltages2::Voltage_Cell11.setPhysicalValue(voltages[10]);
        AMS_CellVoltages2::Voltage_Cell12.setPhysicalValue(voltages[11]);

        //Simulink model
        externalInputs.BMSVoltageInData_c.Voltage1 = voltages[1];
        externalInputs.BMSVoltageInData_c.Voltage2 = voltages[1];
        externalInputs.BMSVoltageInData_c.Voltage3 = voltages[2];
        externalInputs.BMSVoltageInData_c.Voltage4 = voltages[3];
        externalInputs.BMSVoltageInData_c.Voltage5 = voltages[4];
        externalInputs.BMSVoltageInData_c.Voltage6 = voltages[5];
        externalInputs.BMSVoltageInData_c.Voltage7 = voltages[7];
        externalInputs.BMSVoltageInData_c.Voltage8 = voltages[7];
        externalInputs.BMSVoltageInData_c.Voltage9 = voltages[8];
        externalInputs.BMSVoltageInData_c.Voltage10 = voltages[9];
        externalInputs.BMSVoltageInData_c.Voltage11 = voltages[10];
        externalInputs.BMSVoltageInData_c.Voltage12 = voltages[11];
    }

    void readTemperatures(){
        float temperatures[16];
        if(amsManager.readCellTemperature(Device::STACK_DEVICE2, &temperatures[0], 8) != Status::COMPLETED)
            Debug::exception();
        if(amsManager.readCellTemperature(Device::STACK_DEVICE3, &temperatures[8], 8) != Status::COMPLETED)
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

                AMS_Simulink::Min_Temperature.setPhysicalValue(tempMin);
                AMS_Simulink::Max_Temperature.setPhysicalValue(tempMax);

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

        //Simulink model
        externalInputs.BMSTempInData_f.Temp1 = temperatures[0];
        externalInputs.BMSTempInData_f.Temp2 = temperatures[1];
        externalInputs.BMSTempInData_f.Temp3 = temperatures[2];
        externalInputs.BMSTempInData_f.Temp4 = temperatures[3];
        externalInputs.BMSTempInData_f.Temp5 = temperatures[4];
        externalInputs.BMSTempInData_f.Temp6 = temperatures[5];
        externalInputs.BMSTempInData_f.Temp7 = temperatures[9];
        externalInputs.BMSTempInData_f.Temp8 = temperatures[9];
        externalInputs.BMSTempInData_f.Temp9 = temperatures[10];
        externalInputs.BMSTempInData_f.Temp10 = temperatures[11];
        externalInputs.BMSTempInData_f.Temp11 = temperatures[12];
        externalInputs.BMSTempInData_f.Temp12 = temperatures[13];
    }

    void readDieTemperatures(){
        float temperatures[4];
        if(amsManager.readDieTemperature(temperatures) != Status::COMPLETED)
            Debug::exception();

//        AMS_DieTemperatures::Temperature_Die_BD.setPhysicalValue(temperatures[0]);
//        AMS_DieTemperatures::Temperature_Die_S1_D1.setPhysicalValue(temperatures[2]);
//        AMS_DieTemperatures::Temperature_Die_S1_D2.setPhysicalValue(temperatures[3]);
    }

    void readFaults(){
        FaultSummary faults_BD;
        FaultSummary faults_S1_D1;
        FaultSummary faults_S1_D2;

        if(amsManager.getFaultSummary(Device::BASE_DEVICE, &faults_BD) != Status::COMPLETED)
            Debug::exception();
        if(amsManager.getFaultSummary(Device::STACK_DEVICE2, &faults_S1_D1) != Status::COMPLETED)
            Debug::exception();
        if(amsManager.getFaultSummary(Device::STACK_DEVICE3, &faults_S1_D2) != Status::COMPLETED)
            Debug::exception();

//        AMS_Fault_Summary::Fault_Protection_BD.setPhysicalValue(faults_BD.faultProt);
//        AMS_Fault_Summary::Fault_ADC_Comparison_BD.setPhysicalValue(faults_BD.faultCompADC);
//        AMS_Fault_Summary::Fault_OTP_BD.setPhysicalValue(faults_BD.faultOTP);
//        AMS_Fault_Summary::Fault_Communication_BD.setPhysicalValue(faults_BD.faultComm);
//        AMS_Fault_Summary::Fault_OTUT_BD.setPhysicalValue(faults_BD.faultOTUT);
//        AMS_Fault_Summary::Fault_OVOV_BD.setPhysicalValue(faults_BD.faultOVUV);
//        AMS_Fault_Summary::Fault_System_BD.setPhysicalValue(faults_BD.faultSys);
//        AMS_Fault_Summary::Fault_Power_Rail_BD.setPhysicalValue(faults_BD.faultPwr);
//
//        AMS_Fault_Summary::Fault_Protection_S1_D1.setPhysicalValue(faults_S1_D1.faultProt);
//        AMS_Fault_Summary::Fault_ADC_Comparison_S1_D1.setPhysicalValue(faults_S1_D1.faultCompADC);
//        AMS_Fault_Summary::Fault_OTP_S1_D1.setPhysicalValue(faults_S1_D1.faultOTP);
//        AMS_Fault_Summary::Fault_Communication_S1_D1.setPhysicalValue(faults_S1_D1.faultComm);
//        AMS_Fault_Summary::Fault_OTUT_S1_D1.setPhysicalValue(faults_S1_D1.faultOTUT);
//        AMS_Fault_Summary::Fault_OVOV_S1_D1.setPhysicalValue(faults_S1_D1.faultOVUV);
//        AMS_Fault_Summary::Fault_System_S1_D1.setPhysicalValue(faults_S1_D1.faultSys);
//        AMS_Fault_Summary::Fault_Power_Rail_S1_D1.setPhysicalValue(faults_S1_D1.faultPwr);
//
//        AMS_Fault_Summary::Fault_Protection_S1_D2.setPhysicalValue(faults_S1_D2.faultProt);
//        AMS_Fault_Summary::Fault_ADC_Comparison_S1_D2.setPhysicalValue(faults_S1_D2.faultCompADC);
//        AMS_Fault_Summary::Fault_OTP_S1_D2.setPhysicalValue(faults_S1_D2.faultOTP);
//        AMS_Fault_Summary::Fault_Communication_S1_D2.setPhysicalValue(faults_S1_D2.faultComm);
//        AMS_Fault_Summary::Fault_OTUT_S1_D2.setPhysicalValue(faults_S1_D2.faultOTUT);
//        AMS_Fault_Summary::Fault_OVOV_S1_D2.setPhysicalValue(faults_S1_D2.faultOVUV);
//        AMS_Fault_Summary::Fault_System_S1_D2.setPhysicalValue(faults_S1_D2.faultSys);
//        AMS_Fault_Summary::Fault_Power_Rail_S1_D2.setPhysicalValue(faults_S1_D2.faultPwr);
    }

    void readCurrent(){
//        uint64_t temp = IVTS_Current::IVTS_Current.getPhysicalValue();
//
//        AMS_Current::TS_Current.setPhysicalValue(temp);
    }

    void runSimulink(){
        externalInputs.BatteryState = BatteryMode::Driving;
        bms.setExternalInputs(&externalInputs);
        bms.step();
        externalOutputs = bms.getExternalOutputs();

        AMS_Simulink::Simulink_Voltage.setPhysicalValue(externalOutputs.BMSActualValues.Voltage);
        AMS_Simulink::Simulink_BatTemp.setPhysicalValue(externalOutputs.BMSActualValues.BatTemp);
    }
};

void runAMS_Shield(){

    AMS_Shield amsShield(daisyChain);

    amsShield.generalStartup();
    amsShield.startUp();

    for(;;){
        // Diagnosis functionality
        amsShield.readDieTemperatures();
        amsShield.readFaults();

        // Main functionality
        amsShield.readTemperatures();
        amsShield.readVoltages();
        amsShield.readCurrent();

        // Simulink model
        amsShield.runSimulink();

        Parallel::delay(1);
    }
}

void initAMS_Shield(){
    amsShieldTask.init();
}