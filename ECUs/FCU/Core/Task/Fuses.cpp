#include <IO/GPIO/Pin.h>
#include <IO/fuses/EFuse.h>
#include "Init.h"
#include "main.h"
#include <IO/CAN/generated/CANMessages.h>
#include <parallel/Task.hpp>

using namespace Parallel;
using namespace EFuse;

// Forward declaration for Task
[[noreturn]] void runFuses();
Parallel::NormalTask fuseTask(&runFuses, "Fuses");
inline constexpr Parallel::TimeMS INTERVAL_DELAY = 100;

class TPS259570WithSetOnSignal : public TPS259570 {
    CAN::Signal &setSignal;

  public:
    TPS259570WithSetOnSignal(const GPIO::Pin &enablePin, const GPIO::Pin &faultPin, CAN::Signal &enableSignal, CAN::Signal &faultSignal, CAN::Signal &setSignal, bool defaultEnabled = false) :
            TPS259570(enablePin, faultPin, enableSignal, faultSignal, defaultEnabled), setSignal(setSignal) {}

    void setFuseBasedOnCANMessage() {
        if (setSignal.getPhysicalValue() == 1) {
            enableFuse();
        } else if (setSignal.getPhysicalValue() == 0) {
            disableFuse();
        }
        // if value is -1, use the last state -> skip
    }
};

TPS259570WithSetOnSignal eFuses[] = {
        TPS259570WithSetOnSignal(GPIO::Pin(SA_SD_Enable_GPIO_Port, SA_SD_Enable_Pin),
              GPIO::Pin(SA_SD_eFuse_Fault_GPIO_Port,SA_SD_eFuse_Fault_Pin),
              FCU_Fuse_Enable::FCU_Fuse_SA_SD_Enable,
              FCU_Fuse_Fault::FCU_Fuse_SA_SD_Fault,
             FCU_Set_Fuses::FCU_Fuse_SET_SA_SD_Enable,
              true),
        TPS259570WithSetOnSignal(GPIO::Pin(RH_Enable_GPIO_Port, RH_Enable_Pin),
              GPIO::Pin(RH_eFuse_Fault_GPIO_Port, RH_eFuse_Fault_Pin),
              FCU_Fuse_Enable::FCU_Fuse_RH_Enable,
              FCU_Fuse_Fault::FCU_Fuse_RH_Fault,
                FCU_Set_Fuses::FCU_Fuse_SET_RH_Enable,
              true),
        TPS259570WithSetOnSignal(GPIO::Pin(PTU_TKU_TPMS_Enable_GPIO_Port, PTU_TKU_TPMS_Enable_Pin),
                GPIO::Pin(PTU_TKU_TPMS_eFuse_Fault_GPIO_Port, PTU_TKU_TPMS_eFuse_Fault_Pin),
                FCU_Fuse_Enable::FCU_Fuse_PTU_TKU_TPMS_Enable,
                FCU_Fuse_Fault::FCU_Fuse_PTU_TKU_TPMS_Fault,
                 FCU_Set_Fuses::FCU_Fuse_SET_PTU_TKU_TPMS_Enable,
              true),
        TPS259570WithSetOnSignal(GPIO::Pin(Piezo_Enable_GPIO_Port, Piezo_Enable_Pin),
                GPIO::Pin(Piezo_eFuse_Fault_GPIO_Port, Piezo_eFuse_Fault_Pin),
                FCU_Fuse_Enable::FCU_Fuse_Piezo_Enable,
                FCU_Fuse_Fault::FCU_Fuse_Piezo_Fault,
                 FCU_Set_Fuses::FCU_Fuse_SET_Piezo_Enable,
              false),
        TPS259570WithSetOnSignal(GPIO::Pin(IMU_Enable_GPIO_Port, IMU_Enable_Pin),
                GPIO::Pin(IMU_eFuse_Fault_GPIO_Port, IMU_eFuse_Fault_Pin),
                FCU_Fuse_Enable::FCU_Fuse_IMU_Enable,
                FCU_Fuse_Fault::FCU_Fuse_IMU_Fault,
                 FCU_Set_Fuses::FCU_Fuse_SET_IMU_Enable,
              true),
        TPS259570WithSetOnSignal(GPIO::Pin(GPS_Enable_GPIO_Port, GPS_Enable_Pin),
                GPIO::Pin(GPS_eFuse_Fault_GPIO_Port, GPS_eFuse_Fault_Pin),
                FCU_Fuse_Enable::FCU_Fuse_GPS_Enable,
                FCU_Fuse_Fault::FCU_Fuse_GPS_Fault,
                FCU_Set_Fuses::FCU_Fuse_SET_GPS_Enable,
              true),
        TPS259570WithSetOnSignal(GPIO::Pin(SMUs_Enable_GPIO_Port, SMUs_Enable_Pin),
                GPIO::Pin(SMUs_eFuse_Fault_GPIO_Port, SMUs_eFuse_Fault_Pin),
                FCU_Fuse_Enable::FCU_Fuse_SMU_Enable,
                FCU_Fuse_Fault::FCU_Fuse_SMU_Fault,
                FCU_Set_Fuses::FCU_Fuse_SET_SMU_Enable,
              true),
        TPS259570WithSetOnSignal(GPIO::Pin(APPS_Enable_GPIO_Port, APPS_Enable_Pin),
                GPIO::Pin(APPS_eFuse_Fault_GPIO_Port, APPS_eFuse_Fault_Pin),
                FCU_Fuse_Enable::FCU_Fuse_APPS_Enable,
                FCU_Fuse_Fault::FCU_Fuse_APPS_Fault,
                 FCU_Set_Fuses::FCU_Fuse_SET_APPS_Enable,
              true),
};

void setFusesBasedOnCANMessage() {
    for (auto &eFuse : eFuses) {
        eFuse.setFuseBasedOnCANMessage();
    }

}

void runFuses() {
    // Init ADC and Channels
    for (auto &eFuse : eFuses) {
        eFuse.setDefaultState();
    }

    // When the message is received, set the fuses based on the message
    FCU_Set_Fuses::message.setHandler(&setFusesBasedOnCANMessage);


    for (Parallel::Tick lastWakeTime = Parallel::getTick();;
         Parallel::delayUntil(INTERVAL_DELAY, &lastWakeTime)) {
        for (auto &eFuse : eFuses) {
            eFuse.updateStatus();
        }
    }

}

void initFuses() {
    fuseTask.init();
}
