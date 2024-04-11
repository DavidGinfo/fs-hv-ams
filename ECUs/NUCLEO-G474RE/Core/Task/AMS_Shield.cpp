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

//UART::CRXPort uartPortAMSShield = UART::CRXPort::create<&huart4>("GPS-UART-RX");

GPIO::Pin uart_tx = GPIO::Pin(Host_UART_TX_GPIO_Port, Host_UART_TX_Pin);
GPIO::Pin uart_rx = GPIO::Pin(Host_UART_RX_GPIO_Port, Host_UART_RX_Pin);

const uint8_t DEVICE_COUNT = 3;

// Simulink model code
ExtU_BMS_T externalInputs;
BMS bms = BMS();

class AMS_Shield  {
    DaisyChain chain;
    uint8_t lostMessagesCount = 0;

  public:
    explicit AMS_Shield() : chain(&UART::CRXPort::create<&huart4>("AMS-UART-RX"), channelAMSShield, uart_tx, uart_rx){}

    void generalStartup(){
        bool autoAdressingDone = false;

        chain.init(huart4, htim16, DEVICE_COUNT);

        chain.wake();

        chain.getUartCRXPort()->init();
        chain.getUartCRXPort()->setInterruptHandler(UART::InterruptHandler::bind<&BQ79612::DaisyChain::handleUARTInterrupt>(chain));

        //Simulink model
        bms.initialize();

        do{
            if(chain.performAutoadressing() == Status::COMPLETED){
                autoAdressingDone = true;
            }
        }while(!autoAdressingDone);
    }

    void startUp(){
        uint8_t cfg = static_cast<uint8_t>(ADC_Ctrl1::MAIN_MODE_CONTINOUS_RUN) | static_cast<uint8_t>(ADC_Ctrl1::MAIN_GO);

        chain.activateVCs(Device::STACK_DEVICE1, Active_Cell::_6S);
        chain.activateVCs(Device::STACK_DEVICE2, Active_Cell::_6S);
        chain.setActivateGPIOADCs(Device::STACK_DEVICE1);
        chain.setActivateGPIOADCs(Device::STACK_DEVICE2);
        chain.activateADCs(cfg);
    }

    void readVoltages(){
        float voltages[12] = {};
        if(chain.readCellVoltages(Device::STACK_DEVICE1, &voltages[0], 6) != Status::COMPLETED){
            lostMessagesCount++;
            return;
        }
        if(chain.readCellVoltages(Device::STACK_DEVICE2, &voltages[6], 6) != Status::COMPLETED){
            lostMessagesCount++;
            return;
        }

        AMS_CellVoltages1::Voltage_Cell01.setPhysicalValue(voltages[0]);
        AMS_CellVoltages1::Voltage_Cell02.setPhysicalValue(voltages[1]);
        AMS_CellVoltages1::Voltage_Cell03.setPhysicalValue(voltages[2]);
        AMS_CellVoltages1::Voltage_Cell04.setPhysicalValue(voltages[3]);
        AMS_CellVoltages2::Voltage_Cell05.setPhysicalValue(voltages[4]);
        AMS_CellVoltages2::Voltage_Cell06.setPhysicalValue(voltages[5]);
        AMS_CellVoltages2::Voltage_Cell07.setPhysicalValue(voltages[6]);
        AMS_CellVoltages2::Voltage_Cell08.setPhysicalValue(voltages[7]);
        AMS_CellVoltages3::Voltage_Cell09.setPhysicalValue(voltages[8]);
        AMS_CellVoltages3::Voltage_Cell10.setPhysicalValue(voltages[9]);
        AMS_CellVoltages3::Voltage_Cell11.setPhysicalValue(voltages[10]);
        AMS_CellVoltages3::Voltage_Cell12.setPhysicalValue(voltages[11]);

        externalInputs.BMSVoltageInData_c.Voltage1 = voltages[0];
        externalInputs.BMSVoltageInData_c.Voltage2 = voltages[1];
        externalInputs.BMSVoltageInData_c.Voltage3 = voltages[2];
        externalInputs.BMSVoltageInData_c.Voltage4 = voltages[3];
        externalInputs.BMSVoltageInData_c.Voltage5 = voltages[4];
        externalInputs.BMSVoltageInData_c.Voltage6 = voltages[5];
        externalInputs.BMSVoltageInData_c.Voltage7 = voltages[6];
        externalInputs.BMSVoltageInData_c.Voltage8 = voltages[7];
        externalInputs.BMSVoltageInData_c.Voltage9 = voltages[8];
        externalInputs.BMSVoltageInData_c.Voltage10 = voltages[9];
        externalInputs.BMSVoltageInData_c.Voltage11 = voltages[10];
        externalInputs.BMSVoltageInData_c.Voltage12 = voltages[11];
    }

    void readTemperatures(){
        float temperatures[16];
        if(chain.readCellTemperature(Device::STACK_DEVICE1, &temperatures[0], 8) != Status::COMPLETED){
            lostMessagesCount++;
            return;
        }
        if(chain.readCellTemperature(Device::STACK_DEVICE2, &temperatures[8], 8) != Status::COMPLETED){
            lostMessagesCount++;
            return;
        }

        AMS_CellTemperatures1::Temperature_Cell01.setPhysicalValue(temperatures[0]);
        AMS_CellTemperatures1::Temperature_Cell02.setPhysicalValue(temperatures[1]);
        AMS_CellTemperatures1::Temperature_Cell03.setPhysicalValue(temperatures[2]);
        AMS_CellTemperatures1::Temperature_Cell04.setPhysicalValue(temperatures[3]);
        AMS_CellTemperatures1::Temperature_Cell05.setPhysicalValue(temperatures[4]);
        AMS_CellTemperatures1::Temperature_Cell06.setPhysicalValue(temperatures[5]);
        AMS_CellTemperatures2::Temperature_Cell07.setPhysicalValue(temperatures[8]);
        AMS_CellTemperatures2::Temperature_Cell08.setPhysicalValue(temperatures[9]);
        AMS_CellTemperatures2::Temperature_Cell09.setPhysicalValue(temperatures[10]);
        AMS_CellTemperatures2::Temperature_Cell10.setPhysicalValue(temperatures[11]);
        AMS_CellTemperatures2::Temperature_Cell11.setPhysicalValue(temperatures[12]);
        AMS_CellTemperatures2::Temperature_Cell12.setPhysicalValue(temperatures[13]);
        AMS_CellTemperatures_Extra::Temperature_Cell_Extra1.setPhysicalValue(temperatures[6]);
        AMS_CellTemperatures_Extra::Temperature_Cell_Extra2.setPhysicalValue(temperatures[7]);
        AMS_CellTemperatures_Extra::Temperature_Cell_Extra3.setPhysicalValue(temperatures[14]);
        AMS_CellTemperatures_Extra::Temperature_Cell_Extra4.setPhysicalValue(temperatures[15]);

        externalInputs.BMSTempInData_f.Temp1 = temperatures[0];
        externalInputs.BMSTempInData_f.Temp2 = temperatures[1];
        externalInputs.BMSTempInData_f.Temp3 = temperatures[2];
        externalInputs.BMSTempInData_f.Temp4 = temperatures[3];
        externalInputs.BMSTempInData_f.Temp5 = temperatures[4];
        externalInputs.BMSTempInData_f.Temp6 = temperatures[5];
        externalInputs.BMSTempInData_f.Temp7 = temperatures[6];
        externalInputs.BMSTempInData_f.Temp8 = temperatures[7];
        externalInputs.BMSTempInData_f.Temp9 = temperatures[7]; // TODO: set correct input, unconnected NTC, just for simulink testing
        externalInputs.BMSTempInData_f.Temp10 = temperatures[9];
        externalInputs.BMSTempInData_f.Temp11 = temperatures[10];
        externalInputs.BMSTempInData_f.Temp12 = temperatures[11];
    }

    void readCurrent(){
        float temp = IVTS_Current::IVTS_Current.getPhysicalValue();
        AMS_Current::TS_Current.setPhysicalValue(temp);
    }

    void readDieTemperatures(){
        float temperatures[3];
        if(chain.readDieTemperature(temperatures) != Status::COMPLETED){
            lostMessagesCount++;
            return;
        }

        AMS_Diagnosis::Temperature_Die_BD.setPhysicalValue(temperatures[0]);
        AMS_Diagnosis::Temperature_Die_S1_D1.setPhysicalValue(temperatures[1]);
        AMS_Diagnosis::Temperature_Die_S1_D2.setPhysicalValue(temperatures[2]);
    }

    void evaluateSimulinkModel(){
        externalInputs.BatteryState = BatteryMode::Driving;
        externalInputs.BMSCurrentInData = IVTS_Current::IVTS_Current.getPhysicalValue();
        bms.setExternalInputs(&externalInputs);
        bms.step();

        AMS_Simulink::Simulink_BatTemp.setPhysicalValue(bms.getExternalOutputs().BMSActualValues.BatTemp);
        AMS_Simulink::Simulink_Voltage.setPhysicalValue(bms.getExternalOutputs().BMSActualValues.Voltage);
//        AMS_Simulink::Simulink_Voltage.setPhysicalValue(bms.getExternalOutputs().BMSActualValues.BatCurrent);
//        AMS_Simulink::Simulink_Voltage.setPhysicalValue(bms.getExternalOutputs().BMSActualValues.BatSOC);
    }

    void performDiagnosis(){
        this->readDieTemperatures();

        AMS_Diagnosis::Lost_Messages_Counter.setPhysicalValue(lostMessagesCount);
    }
};

void runAMS_Shield(){

    AMS_Shield amsShield;

    amsShield.generalStartup();
    amsShield.startUp();

    for(;;){
        // Diagnosis functionality
        amsShield.performDiagnosis();

        // Main functionality
        amsShield.readTemperatures();
        amsShield.readVoltages();
        amsShield.readCurrent();

        //Simulink functionality
        amsShield.evaluateSimulinkModel();

        Parallel::delay(1);
    }
}

void initAMS_Shield(){
    amsShieldTask.init();
}