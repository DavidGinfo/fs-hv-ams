#pragma once

#include <util/Common.h>

namespace BQ79612 {
    struct FaultSummary{
        [[maybe_unused]] bool faultProt;
        [[maybe_unused]] bool faultCompADC;
        [[maybe_unused]] bool faultOTP;
        [[maybe_unused]] bool faultComm;
        [[maybe_unused]] bool faultOTUT;
        [[maybe_unused]] bool faultOVUV;
        [[maybe_unused]] bool faultSys;
        [[maybe_unused]] bool faultPwr;
    };

}