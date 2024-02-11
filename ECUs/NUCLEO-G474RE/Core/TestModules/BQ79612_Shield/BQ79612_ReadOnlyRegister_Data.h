#pragma once

#include <util/Common.h>

namespace BQ79612 {
    struct [[maybe_unused]] FaultSummary{
        [[maybe_unused]] bool faultProt;
        [[maybe_unused]] bool faultCompADC;
        [[maybe_unused]] bool faultOTP;
        [[maybe_unused]] bool faultComm;
        [[maybe_unused]] bool faultOTUT;
        [[maybe_unused]] bool faultOVUV;
        [[maybe_unused]] bool faultSys;
        [[maybe_unused]] bool faultPwr;
    };

    struct [[maybe_unused]] FaultComm{
        [[maybe_unused]] bool uartTR;
        [[maybe_unused]] bool uartRR;
        [[maybe_unused]] bool uartRC;
        [[maybe_unused]] bool commClrDet;
        [[maybe_unused]] bool stopDet;
        [[maybe_unused]] bool comLTR;
        [[maybe_unused]] bool comLRR;
        [[maybe_unused]] bool comLRC;
        [[maybe_unused]] bool comLBit;
        [[maybe_unused]] bool comHTR;
        [[maybe_unused]] bool comHRR;
        [[maybe_unused]] bool comHRC;
        [[maybe_unused]] bool comHBit;
        [[maybe_unused]] bool fCommDet;
        [[maybe_unused]] bool fToneDet;
        [[maybe_unused]] bool hbFail;
        [[maybe_unused]] bool hbFast;
    };

    struct [[maybe_unused]] FaultOTP{
        [[maybe_unused]] bool xxx;
    };

    struct [[maybe_unused]] FaultSys{
        [[maybe_unused]] bool xxx;
    };

    struct [[maybe_unused]] FaultProt{
        [[maybe_unused]] bool xxx;
    };

    struct [[maybe_unused]] FaultOV{
        [[maybe_unused]] bool xxx;
    };

    struct [[maybe_unused]] FaultUV{
        [[maybe_unused]] bool xxx;
    };

    struct [[maybe_unused]] FaultOT{
        [[maybe_unused]] bool xxx;
    };

    struct [[maybe_unused]] FaultUT{
        [[maybe_unused]] bool xxx;
    };

    struct [[maybe_unused]] FaultCompGPIO{
        [[maybe_unused]] bool xxx;
    };

    struct [[maybe_unused]] FaultCompVCCB{
        [[maybe_unused]] bool xxx;
    };

    struct [[maybe_unused]] FaultCompVC0W{
        [[maybe_unused]] bool xxx;
    };

    struct [[maybe_unused]] FaultCompVBOW{
        [[maybe_unused]] bool xxx;
    };

    struct [[maybe_unused]] FaultCompCBFET{
        [[maybe_unused]] bool xxx;
    };

    struct [[maybe_unused]] FaultCompMisc{
        [[maybe_unused]] bool xxx;
    };

    struct [[maybe_unused]] FaultPwr{
        [[maybe_unused]] bool xxx;
    };
}