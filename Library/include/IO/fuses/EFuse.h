#pragma once

#include <IO/GPIO/Pin.h>
#include <IO/CAN/Signal.h>


namespace EFuse {
    class BaseEFuse {
        GPIO::Pin enablePin;
        CAN::Signal &enableSignal;
        bool defaultEnabled;

      public:
        BaseEFuse(const GPIO::Pin &enablePin, CAN::Signal &enableSignal, bool defaultEnabled = false) :
                  enablePin(enablePin), enableSignal(enableSignal), defaultEnabled(defaultEnabled) {}

        void enableFuse();

        void disableFuse();

        void setDefaultState();
    };

    class TPS259570 : public BaseEFuse {
        // Extend base class with fault signal / pin and updateStatus function
        GPIO::Pin faultPin;
        CAN::Signal &faultSignal;

      public:
        TPS259570(const GPIO::Pin &enablePin, const GPIO::Pin &faultPin, CAN::Signal &enableSignal, CAN::Signal &faultSignal, bool defaultEnabled = false) :
                  BaseEFuse(enablePin, enableSignal, defaultEnabled), faultPin(faultPin), faultSignal(faultSignal) {}

        void updateStatus();

    };

}