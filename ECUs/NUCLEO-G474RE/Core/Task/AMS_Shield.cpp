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
        float voltages[6*(DEVICE_COUNT-1)] = {};
        if(chain.readCellVoltages(Device::STACK_DEVICE1, &voltages[0], 6) != Status::COMPLETED){
            lostMessagesCount++;
            return;
        }
        if(chain.readCellVoltages(Device::STACK_DEVICE2, &voltages[6], 6) != Status::COMPLETED){
            lostMessagesCount++;
            return;
        }
    }

    void readTemperatures(){
        float temperatures[8*(DEVICE_COUNT-1)];
        if(chain.readCellTemperature(Device::STACK_DEVICE1, &temperatures[0], 8) != Status::COMPLETED){
            lostMessagesCount++;
            return;
        }
        if(chain.readCellTemperature(Device::STACK_DEVICE2, &temperatures[8], 8) != Status::COMPLETED){
            lostMessagesCount++;
            return;
        }
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