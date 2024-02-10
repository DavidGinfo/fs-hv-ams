#pragma once

#include <util/Common.h>

namespace BQ79612 {
    const uint8_t RESET_VALUE_0 = 0x00;

    enum class Active_Cell : uint8_t {
        _6S         = 0x0,
        _7S         = 0x1,
        _8S         = 0x2,
        _9S         = 0x3,
        _10S        = 0x4,
        _11S        = 0x5,
        _12S        = 0x6,
        RESET_VALUE = 0x00
    };

    enum class GPIO_Conf1 : uint8_t {
        RESET_VALUE = 0x00
    };

    enum class GPIO_Conf2 : uint8_t {
        RESET_VALUE = 0x00
    };

    enum class GPIO_Conf3 : uint8_t {
        RESET_VALUE = 0x00
    };

    enum class GPIO_Conf4 : uint8_t {
        GPIO8_DISABLED            = 0b000 << 3,
        GPIO8_ADC_OTUT            = 0b001 << 3,
        GPIO8_ADC_ONLY            = 0b010 << 3,
        GPIO8_DIGITAL_INPUT       = 0b011 << 3,
        GPIO8_OUTPUT_HIGH         = 0b100 << 3,
        GPIO8_OUTPUT_LOW          = 0b101 << 3,
        GPIO8_ADC_PULL_UP         = 0b110 << 3,
        GPIO8_ADC_PULL_DOWN       = 0b111 << 3,
        GPIO7_DISABLED            = 0b000,
        GPIO7_ADC_OTUT            = 0b001,
        GPIO7_ADC_ONLY            = 0b010,
        GPIO7_DIGITAL_INPUT       = 0b011,
        GPIO7_OUTPUT_HIGH         = 0b100,
        GPIO7_OUTPUT_LOW          = 0b101,
        GPIO7_ADC_PULL_UP         = 0b110,
        GPIO7_ADC_PULL_DOWN       = 0b111,
        RESET_VALUE               = 0x00
    };

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