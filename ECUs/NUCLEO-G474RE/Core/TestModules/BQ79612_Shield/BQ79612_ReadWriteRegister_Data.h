#pragma once

#include <util/Common.h>

namespace BQ79612 {

    enum class Comm_Ctrl : uint8_t {
        STACK_DEV = 1 << 1,
        TOP_STACK = 1,
        RESET_VALUE = 0x00
    };

    enum class Control1 : uint8_t {
        DIR_SEL       = 1 << 7,
        SEND_SHUTDOWN = 1 << 6,
        SEND_WAKE     = 1 << 5,
        SEND_SLPTOACT = 1 << 4,
        GOTO_SHUTDOWN = 1 << 3,
        GOTO_SLEEP    = 1 << 2,
        SOFT_RESET    = 1 << 1,
        ADDR_WR       = 1,
        RESET_VALUE   = 0x00
    };

    enum class Control2 : uint8_t {
        SEND_HW_RESET = 1 << 1,
        TSREF_EN      = 1,
        RESET_VALUE   = 0x00
    };

    enum class ADC_Ctrl1 : uint8_t {
        LPF_BB_EN               = 0b1 << 4,
        LPF_VCELL_EN            = 0b1 << 3,
        MAIN_GO                 = 0b1 << 2,
        MAIN_MODE_NOT_RUNNING   = 0b00,
        MAIN_MODE_SINGLE_RUN    = 0b01,
        MAIN_MODE_CONTINOUS_RUN = 0b10,
        RESET_VALUE             = 0x00
    };

    enum class OTP_ECC_Test : uint8_t {
        DED_SEC     = 1 << 3,
        MANUAL_AUTO = 1 << 2,
        ENC_DEC     = 1 << 1,
        ENABLE      = 1,
        RESET_VALUE = 0x00
    };
}