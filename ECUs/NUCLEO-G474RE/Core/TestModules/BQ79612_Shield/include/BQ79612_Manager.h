#pragma once

#include "BQ79612.h"

namespace BQ79612 {


    class Manager{
      public:
        Manager(Manager &) = delete;
        Manager(Manager &&) = delete;

        Manager(DaisyChain daisyChain);

        void wake();
        void sleep();
        void shutdown();

        Status performAutoadressing();
        Status activateADCs(uint8_t config);
        Status activateVCs(Device device, Active_Cell activeVCConfig);
        Status readCellVoltages(Device device, float *cellVoltages, uint8_t cellVoltageCount);
        Status setActivateGPIOADCs(Device device, uint8_t activeGPIOCount);
        Status readCellTemperature(Device device, float *cellTemperatures, uint8_t cellTemperatureCount);
        Status readDieTemperature(float *dieTemperature);
        Status getFaultSummary(Device device, FaultSummary *faults);
      private:
        DaisyChain daisyChain;


    };
}
