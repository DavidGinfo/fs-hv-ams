#pragma once

#include <util/Common.h>

namespace BQ79612 {
    struct FaultSummary{
        [[maybe_unused]] bool FAULT_PROT;
        [[maybe_unused]] bool FAULT_COMP_ADC;
        [[maybe_unused]] bool FAULT_OTP;
        [[maybe_unused]] bool FAULT_COMM;
        [[maybe_unused]] bool FAULT_OTUT;
        [[maybe_unused]] bool FAULT_OVUV;
        [[maybe_unused]] bool FAULT_SYS;
        [[maybe_unused]] bool FAULT_PWR;
    };

}