#pragma once

namespace BQ79612 {
    const uint8_t REGISTER_DEFAULT_VALUE_00 = 0x00;

    enum ReadWriteRegister : uint16_t {
        OTP_ECC_TEST = 0x034C,
        Control1     = 0x0309,
        DIR0_ADDR    = 0x0306,
        COMM_CTRL    = 0x0308,
        ADC_CTRL1    = 0x030D,
        ADC_CTRL2    = 0x030E
    };

    enum ReadOnlyRegister : uint16_t {
        PART_ID       = 0x0500,
        FAULT_SUMMERY = 0x052D,
        VCELL12_HI    = 0x0570,
        VCELL12_LO    = 0x0571,
        VCELL11_HI    = 0x0572,
        VCELL11_LO    = 0x0573,
        VCELL10_HI    = 0x0574,
        VCELL10_LO    = 0x0575,
        VCELL9_HI     = 0x0576,
        VCELL9_LO     = 0x0577,
        VCELL8_HI     = 0x0578,
        VCELL8_LO     = 0x0579,
        VCELL7_HI     = 0x057A,
        VCELL7_LO     = 0x057B,
        VCELL6_HI     = 0x057C,
        VCELL6_LO     = 0x057D,
        VCELL5_HI     = 0x057E,
        VCELL5_LO     = 0x057F,
        VCELL4_HI     = 0x0580,
        VCELL4_LO     = 0x0581,
        VCELL3_HI     = 0x0582,
        VCELL3_LO     = 0x0583,
        VCELL2_HI     = 0x0584,
        VCELL2_LO     = 0x0585,
        VCELL1_HI     = 0x0586,
        VCELL1_LO     = 0x0587
    };

    enum OTPRegister : uint16_t {
        ACTIVE_CELL = 0x0003
    };

    enum Control1 : uint8_t {
        DIR_SEL       = 1 << 7,
        SEND_SHUTDOWN = 1 << 6,
        SEND_WAKE     = 1 << 5,
        SEND_SLPTOACT = 1 << 4,
        GOTO_SHUTDOWN = 1 << 3,
        GOTO_SLEEP    = 1 << 2,
        SOFT_RESET    = 1 << 1,
        ADDR_WR       = 1
    };

    enum OTP_ECC_Test : uint8_t {
        DED_SEC     = 1 << 3,
        MANUAL_AUTO = 1 << 2,
        ENC_DEC     = 1 << 1,
        ENABLE      = 1,
    };

    enum Comm_Ctrl : uint8_t {
        STACK_DEV = 1 << 1,
        TOP_STACK = 1
    };

    enum Active_Cell : uint8_t {
        _6S  = 0x0,
        _7S  = 0x1,
        _8S  = 0x2,
        _9S  = 0x3,
        _10S = 0x4,
        _11S = 0x5,
        _12S = 0x6,
    };

    enum ADC_Ctrl1 : uint8_t {
        LPF_BB_EN               = 0b1 << 4,
        LPF_VCELL_EN            = 0b1 << 3,
        MAIN_GO                 = 0b1 << 2,
        MAIN_MODE_NOT_RUNNING   = 0b00,
        MAIN_MODE_SINGLE_RUN    = 0b01,
        MAIN_MODE_CONTINOUS_RUN = 0b10
    };
}