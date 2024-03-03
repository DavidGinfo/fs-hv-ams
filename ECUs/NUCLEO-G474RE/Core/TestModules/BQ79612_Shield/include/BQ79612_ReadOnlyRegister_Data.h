#pragma once

#include "util/Common.h"

namespace BQ79612 {
    struct [[maybe_unused]] OTPProgStat{
        [[maybe_unused]] bool unlock;
        [[maybe_unused]] bool otErr;
        [[maybe_unused]] bool uvErr;
        [[maybe_unused]] bool ovErr;
        [[maybe_unused]] bool suvErr;
        [[maybe_unused]] bool sovErr;
        [[maybe_unused]] bool progErr;
        [[maybe_unused]] bool done;
    };

    struct [[maybe_unused]] OTPCust1Stat{
        [[maybe_unused]] bool loaded;
        [[maybe_unused]] bool loadWrn;
        [[maybe_unused]] bool loadErr;
        [[maybe_unused]] bool fmtErr;
        [[maybe_unused]] bool progOK;
        [[maybe_unused]] bool uvOK;
        [[maybe_unused]] bool ovOK;
        [[maybe_unused]] bool _try;
    };

    struct [[maybe_unused]] OTPCust2Stat{
        [[maybe_unused]] bool loaded;
        [[maybe_unused]] bool loadWrn;
        [[maybe_unused]] bool loadErr;
        [[maybe_unused]] bool fmtErr;
        [[maybe_unused]] bool progOK;
        [[maybe_unused]] bool uvOK;
        [[maybe_unused]] bool ovOK;
        [[maybe_unused]] bool _try;
    };

    struct [[maybe_unused]] DiagStat{
        [[maybe_unused]] bool dRdyOTUT;
        [[maybe_unused]] bool dRdyOVUV;
        [[maybe_unused]] bool dRdyBistOTUT;
        [[maybe_unused]] bool dRdyBistOVUV;
        [[maybe_unused]] bool dRdyBistPwr;
    };

    struct [[maybe_unused]] ADCStat{
        [[maybe_unused]] bool dRdyAuxGPIO;
        [[maybe_unused]] bool dRdyAuxCell;
        [[maybe_unused]] bool dRdyAuxMisc;
        [[maybe_unused]] bool dRdyMainADC;
        [[maybe_unused]] bool dRdyLPF;
        [[maybe_unused]] bool dRdyGPIO;
        [[maybe_unused]] bool dRdyVCOW;
        [[maybe_unused]] bool dRdyCBOW;
        [[maybe_unused]] bool dRdyCBFet;
        [[maybe_unused]] bool dRdyVCCB;
    };

    struct [[maybe_unused]] BalStat{
        [[maybe_unused]] bool invalidCBConf;
        [[maybe_unused]] bool otPauseDet;
        [[maybe_unused]] bool cbInPause;
        [[maybe_unused]] bool mbRun;
        [[maybe_unused]] bool cbRun;
        [[maybe_unused]] bool abortFlt;
        [[maybe_unused]] bool mbDone;
        [[maybe_unused]] bool cbDone;
    };

    struct [[maybe_unused]] DevStat{
        [[maybe_unused]] bool factCRCDone;
        [[maybe_unused]] bool custCRCDone;
        [[maybe_unused]] bool otutRun;
        [[maybe_unused]] bool ovuvRun;
        [[maybe_unused]] bool auxRun;
        [[maybe_unused]] bool mainRun;
    };

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
        [[maybe_unused]] bool dedDet;
        [[maybe_unused]] bool secDet;
        [[maybe_unused]] bool custCrc;
        [[maybe_unused]] bool factCrc;
        [[maybe_unused]] bool custLdErr;
        [[maybe_unused]] bool factLdErr;
        [[maybe_unused]] bool gblOVErr;
    };

    struct [[maybe_unused]] FaultSys{
        [[maybe_unused]] bool lfo;
        [[maybe_unused]] bool gpio;
        [[maybe_unused]] bool dRst;
        [[maybe_unused]] bool ctl;
        [[maybe_unused]] bool cts;
        [[maybe_unused]] bool tShut;
        [[maybe_unused]] bool tWarn;
    };

    struct [[maybe_unused]] FaultProt{
        [[maybe_unused]] bool tParityFail;
        [[maybe_unused]] bool vParityFail;
        [[maybe_unused]] bool bistAbort;
        [[maybe_unused]] bool tPathFail;
        [[maybe_unused]] bool vPathFail;
        [[maybe_unused]] bool utCompFail;
        [[maybe_unused]] bool otCompFail;
        [[maybe_unused]] bool ovCompFail;
        [[maybe_unused]] bool uvCompFail;
    };

    struct [[maybe_unused]] FaultOV{
        [[maybe_unused]] bool ov12Det;
        [[maybe_unused]] bool ov11Det;
        [[maybe_unused]] bool ov10Det;
        [[maybe_unused]] bool ov9Det;
        [[maybe_unused]] bool ov8Det;
        [[maybe_unused]] bool ov7Det;
        [[maybe_unused]] bool ov6Det;
        [[maybe_unused]] bool ov5Det;
        [[maybe_unused]] bool ov4Det;
        [[maybe_unused]] bool ov3Det;
        [[maybe_unused]] bool ov2Det;
        [[maybe_unused]] bool ov1Det;
    };

    struct [[maybe_unused]] FaultUV{
        [[maybe_unused]] bool uv12Det;
        [[maybe_unused]] bool uv11Det;
        [[maybe_unused]] bool uv10Det;
        [[maybe_unused]] bool uv9Det;
        [[maybe_unused]] bool uv8Det;
        [[maybe_unused]] bool uv7Det;
        [[maybe_unused]] bool uv6Det;
        [[maybe_unused]] bool uv5Det;
        [[maybe_unused]] bool uv4Det;
        [[maybe_unused]] bool uv3Det;
        [[maybe_unused]] bool uv2Det;
        [[maybe_unused]] bool uv1Det;
    };

    struct [[maybe_unused]] FaultOT{
        [[maybe_unused]] bool ot8Det;
        [[maybe_unused]] bool ot7Det;
        [[maybe_unused]] bool ot6Det;
        [[maybe_unused]] bool ot5Det;
        [[maybe_unused]] bool ot4Det;
        [[maybe_unused]] bool ot3Det;
        [[maybe_unused]] bool ot2Det;
        [[maybe_unused]] bool ot1Det;
    };

    struct [[maybe_unused]] FaultUT{
        [[maybe_unused]] bool ut8Det;
        [[maybe_unused]] bool ut7Det;
        [[maybe_unused]] bool ut6Det;
        [[maybe_unused]] bool ut5Det;
        [[maybe_unused]] bool ut4Det;
        [[maybe_unused]] bool ut3Det;
        [[maybe_unused]] bool ut2Det;
        [[maybe_unused]] bool ut1Det;
    };

    struct [[maybe_unused]] FaultCompGPIO{
        [[maybe_unused]] bool gpio8Fail;
        [[maybe_unused]] bool gpio7Fail;
        [[maybe_unused]] bool gpio6Fail;
        [[maybe_unused]] bool gpio5Fail;
        [[maybe_unused]] bool gpio4Fail;
        [[maybe_unused]] bool gpio3Fail;
        [[maybe_unused]] bool gpio2Fail;
        [[maybe_unused]] bool gpio1Fail;
    };

    struct [[maybe_unused]] FaultCompVCCB{
        [[maybe_unused]] bool cell12Fail;
        [[maybe_unused]] bool cell11Fail;
        [[maybe_unused]] bool cell10Fail;
        [[maybe_unused]] bool cell9Fail;
        [[maybe_unused]] bool cell8Fail;
        [[maybe_unused]] bool cell7Fail;
        [[maybe_unused]] bool cell6Fail;
        [[maybe_unused]] bool cell5Fail;
        [[maybe_unused]] bool cell4Fail;
        [[maybe_unused]] bool cell3Fail;
        [[maybe_unused]] bool cell2Fail;
        [[maybe_unused]] bool cell1Fail;
    };

    struct [[maybe_unused]] FaultCompVC0W{
        [[maybe_unused]] bool vcOW12Fail;
        [[maybe_unused]] bool vcOW11Fail;
        [[maybe_unused]] bool vcOW10Fail;
        [[maybe_unused]] bool vcOW9Fail;
        [[maybe_unused]] bool vcOW8Fail;
        [[maybe_unused]] bool vcOW7Fail;
        [[maybe_unused]] bool vcOW6Fail;
        [[maybe_unused]] bool vcOW5Fail;
        [[maybe_unused]] bool vcOW4Fail;
        [[maybe_unused]] bool vcOW3Fail;
        [[maybe_unused]] bool vcOW2Fail;
        [[maybe_unused]] bool vcOW1Fail;
    };

    struct [[maybe_unused]] FaultCompVBOW{
        [[maybe_unused]] bool vbOW12Fail;
        [[maybe_unused]] bool vbOW11Fail;
        [[maybe_unused]] bool vbOW10Fail;
        [[maybe_unused]] bool vbOW9Fail;
        [[maybe_unused]] bool vbOW8Fail;
        [[maybe_unused]] bool vbOW7Fail;
        [[maybe_unused]] bool vbOW6Fail;
        [[maybe_unused]] bool vbOW5Fail;
        [[maybe_unused]] bool vbOW4Fail;
        [[maybe_unused]] bool vbOW3Fail;
        [[maybe_unused]] bool vbOW2Fail;
        [[maybe_unused]] bool vbOW1Fail;
    };

    struct [[maybe_unused]] FaultCompCBFET{
        [[maybe_unused]] bool sbFet12Fail;
        [[maybe_unused]] bool sbFet11Fail;
        [[maybe_unused]] bool sbFet10Fail;
        [[maybe_unused]] bool sbFet9Fail;
        [[maybe_unused]] bool sbFet8Fail;
        [[maybe_unused]] bool sbFet7Fail;
        [[maybe_unused]] bool sbFet6Fail;
        [[maybe_unused]] bool sbFet5Fail;
        [[maybe_unused]] bool sbFet4Fail;
        [[maybe_unused]] bool sbFet3Fail;
        [[maybe_unused]] bool sbFet2Fail;
        [[maybe_unused]] bool sbFet1Fail;
    };

    struct [[maybe_unused]] FaultCompMisc{
        [[maybe_unused]] bool compAdcAbort;
        [[maybe_unused]] bool lpfFail;
    };

    struct [[maybe_unused]] FaultPwr{
        [[maybe_unused]] bool cvssOpen;
        [[maybe_unused]] bool dvssOpen;
        [[maybe_unused]] bool refhmOpen;
        [[maybe_unused]] bool cvddUV;
        [[maybe_unused]] bool cvddOV;
        [[maybe_unused]] bool dvddOV;
        [[maybe_unused]] bool avddOsc;
        [[maybe_unused]] bool avddOV;
        [[maybe_unused]] bool pwrBistFail;
        [[maybe_unused]] bool refhOsc;
        [[maybe_unused]] bool neg5VUV;
        [[maybe_unused]] bool tsrefOsc;
        [[maybe_unused]] bool tsrefUV;
        [[maybe_unused]] bool tsrefOV;
        [[maybe_unused]] bool avddUVDRst;
    };

    struct [[maybe_unused]] CBComplete{
        [[maybe_unused]] bool cell12Done;
        [[maybe_unused]] bool cell11Done;
        [[maybe_unused]] bool cell10Done;
        [[maybe_unused]] bool cell9Done;
        [[maybe_unused]] bool cell8Done;
        [[maybe_unused]] bool cell7Done;
        [[maybe_unused]] bool cell6Done;
        [[maybe_unused]] bool cell5Done;
        [[maybe_unused]] bool cell4Done;
        [[maybe_unused]] bool cell3Done;
        [[maybe_unused]] bool cell2Done;
        [[maybe_unused]] bool cell1Done;
    };
}