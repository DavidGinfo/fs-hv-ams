#pragma once

#include "util/Common.h"

namespace BQ79612 {
    const uint8_t RESET_VALUE_0 = 0x00;

    enum class Dev_Conf : uint8_t {
        NO_ADJ_CB    = 1 << 6,
        MULTIDROP_EN = 1 << 5,
        FCOMM_EN     = 1 << 4,
        TWO_STOP_EN  = 1 << 3,
        NFAULT_EN    = 1 << 2,
        FTONE_EN     = 1 << 1,
        HB_EN        = 1,
        RESET_VALUE  = 0x54
    };

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

    enum class BBVC_Posn1 : uint8_t {
        CELL12       = 1 << 3,
        CELL11       = 1 << 2,
        CELL10       = 1 << 1,
        CELL9        = 1,
        RESET_VALUE  = 0x00
    };

    enum class BBVC_Posn2 : uint8_t {
        CELL8       = 1 << 7,
        CELL7       = 1 << 6,
        CELL6       = 1 << 5,
        CELL5       = 1 << 4,
        CELL4       = 1 << 3,
        CELL3       = 1 << 2,
        CELL2       = 1 << 1,
        CELL1       = 1,
        RESET_VALUE  = 0x00
    };

    enum class ADC_Conf1 : uint8_t {
        AUX_SETTLE_4300_US    = 0b00 << 6,
        AUX_SETTLE_2300_US    = 0b01 << 6,
        AUX_SETTLE_1300_US    = 0b10 << 6,
        LPF_BB_6_5_HZ         = 0x0 << 3,
        LPF_BB_13_HZ          = 0x1 << 3,
        LPF_BB_26_HZ          = 0x2 << 3,
        LPF_BB_53_HZ          = 0x3 << 3,
        LPF_BB_111_HZ         = 0x4 << 3,
        LPF_BB_240_HZ         = 0x5 << 3,
        LPF_BB_600_HZ         = 0x6 << 3,
        LPF_VCELL_6_5_HZ      = 0x0,
        LPF_VCELL_13_HZ       = 0x1,
        LPF_VCELL_26_HZ       = 0x2,
        LPF_VCELL_53_HZ       = 0x3,
        LPF_VCELL_111_HZ      = 0x4,
        LPF_VCELL_240_HZ      = 0x5,
        LPF_VCELL_600_HZ      = 0x6
    };

    enum class UV_Disable1 : uint8_t {
        CELL12       = 1 << 3,
        CELL11       = 1 << 2,
        CELL10       = 1 << 1,
        CELL9        = 1,
        RESET_VALUE  = 0x00
    };

    enum class UV_Disable2 : uint8_t {
        CELL8       = 1 << 7,
        CELL7       = 1 << 6,
        CELL6       = 1 << 5,
        CELL5       = 1 << 4,
        CELL4       = 1 << 3,
        CELL3       = 1 << 2,
        CELL2       = 1 << 1,
        CELL1       = 1,
        RESET_VALUE  = 0x00
    };

    enum class GPIO_Conf1 : uint8_t {
        FAULT_IN_EN               = 1 << 7,
        SPI_EN                    = 1 << 6,
        GPIO2_DISABLED            = 0b000 << 3,
        GPIO2_ADC_OTUT            = 0b001 << 3,
        GPIO2_ADC_ONLY            = 0b010 << 3,
        GPIO2_DIGITAL_INPUT       = 0b011 << 3,
        GPIO2_OUTPUT_HIGH         = 0b100 << 3,
        GPIO2_OUTPUT_LOW          = 0b101 << 3,
        GPIO2_ADC_PULL_UP         = 0b110 << 3,
        GPIO2_ADC_PULL_DOWN       = 0b111 << 3,
        GPIO1_DISABLED            = 0b000,
        GPIO1_ADC_OTUT            = 0b001,
        GPIO1_ADC_ONLY            = 0b010,
        GPIO1_DIGITAL_INPUT       = 0b011,
        GPIO1_OUTPUT_HIGH         = 0b100,
        GPIO1_OUTPUT_LOW          = 0b101,
        GPIO1_ADC_PULL_UP         = 0b110,
        GPIO1_ADC_PULL_DOWN       = 0b111,
        RESET_VALUE               = 0x00
    };

    enum class GPIO_Conf2 : uint8_t {
        GPIO4_DISABLED            = 0b000 << 3,
        GPIO4_ADC_OTUT            = 0b001 << 3,
        GPIO4_ADC_ONLY            = 0b010 << 3,
        GPIO4_DIGITAL_INPUT       = 0b011 << 3,
        GPIO4_OUTPUT_HIGH         = 0b100 << 3,
        GPIO4_OUTPUT_LOW          = 0b101 << 3,
        GPIO4_ADC_PULL_UP         = 0b110 << 3,
        GPIO4_ADC_PULL_DOWN       = 0b111 << 3,
        GPIO3_DISABLED            = 0b000,
        GPIO3_ADC_OTUT            = 0b001,
        GPIO3_ADC_ONLY            = 0b010,
        GPIO3_DIGITAL_INPUT       = 0b011,
        GPIO3_OUTPUT_HIGH         = 0b100,
        GPIO3_OUTPUT_LOW          = 0b101,
        GPIO3_ADC_PULL_UP         = 0b110,
        GPIO3_ADC_PULL_DOWN       = 0b111,
        RESET_VALUE               = 0x00
    };

    enum class GPIO_Conf3 : uint8_t {
        GPIO6_DISABLED            = 0b000 << 3,
        GPIO6_ADC_OTUT            = 0b001 << 3,
        GPIO6_ADC_ONLY            = 0b010 << 3,
        GPIO6_DIGITAL_INPUT       = 0b011 << 3,
        GPIO6_OUTPUT_HIGH         = 0b100 << 3,
        GPIO6_OUTPUT_LOW          = 0b101 << 3,
        GPIO6_ADC_PULL_UP         = 0b110 << 3,
        GPIO6_ADC_PULL_DOWN       = 0b111 << 3,
        GPIO5_DISABLED            = 0b000,
        GPIO5_ADC_OTUT            = 0b001,
        GPIO5_ADC_ONLY            = 0b010,
        GPIO5_DIGITAL_INPUT       = 0b011,
        GPIO5_OUTPUT_HIGH         = 0b100,
        GPIO5_OUTPUT_LOW          = 0b101,
        GPIO5_ADC_PULL_UP         = 0b110,
        GPIO5_ADC_PULL_DOWN       = 0b111,
        RESET_VALUE               = 0x00
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

    enum class Fault_Msk1 : uint8_t {
        MSK_PROT    = 1 << 7,
        MSK_UT      = 1 << 6,
        MSK_OT      = 1 << 5,
        MSK_UV      = 1 << 4,
        MSK_OV      = 1 << 3,
        MSK_COMP    = 1 << 2,
        MSK_SYS     = 1 << 1,
        MSK_PWR     = 1,
        RESET_VALUE = 0x00
    };

    enum class Fault_Msk2 : uint8_t {
        MSK_OTP_CRC       = 1 << 6,
        MSK_OTP_DATA      = 1 << 5,
        MSK_COMM3_FCOMM   = 1 << 4,
        MSK_COMM3_FTONE   = 1 << 3,
        MSK_COMM3_HB      = 1 << 2,
        MSK_COMM2         = 1 << 1,
        MSK_COMM1         = 1,
        RESET_VALUE       = 0x00
    };

    enum class PWR_Transit_Conf : uint8_t {
        TWARN_THR_85C  = 0b00 << 3,
        TWARN_THR_95C  = 0b01 << 3,
        TWARN_THR_105C = 0b10 << 3,
        TWARN_THR_115C = 0b11 << 3,
        SLP_TIME_0s    = 0b000,
        SLP_TIME_5s    = 0b001,
        SLP_TIME_10s   = 0b010,
        SLP_TIME_1min  = 0b011,
        SLP_TIME_10min = 0b100,
        SLP_TIME_30min = 0b101,
        SLP_TIME_1h    = 0b110,
        SLP_TIME_2h    = 0b111,
        RESET_VALUE    = 0x18
    };

    enum class Comm_Timeout_Conf : uint8_t {
        CTS_TIME_DISABLED = 0b000 << 4,
        CTS_TIME_100ms    = 0b001 << 4,
        CTS_TIME_2s       = 0b010 << 4,
        CTS_TIME_10s      = 0b011 << 4,
        CTS_TIME_1min     = 0b100 << 4,
        CTS_TIME_10min    = 0b101 << 4,
        CTS_TIME_30min    = 0b110 << 4,
        CTS_TIME_1h       = 0b111 << 4,
        CTL_ACT           = 1 << 3,
        CTL_TIME_DISABLED = 0b000,
        CTL_TIME_100ms    = 0b001,
        CTL_TIME_2s       = 0b010,
        CTL_TIME_10s      = 0b011,
        CTL_TIME_1min     = 0b100,
        CTL_TIME_10min    = 0b101,
        CTL_TIME_30min    = 0b110,
        CTL_TIME_1h       = 0b111,
        RESET_VALUE       = 0x00
    };

    enum class BBP_Loc : uint8_t {
        BBP_BBN_UNUSED      = 0x00,
        BBPCELL2_BBNVC1     = 0x01,
        BBPCELL3_BBNVC2     = 0x02,
        BBPCELL4_BBNVC3     = 0x03,
        BBPCELL5_BBNVC4     = 0x04,
        BBPCELL6_BBNVC5     = 0x05,
        BBPCELL7_BBNVC6     = 0x06,
        BBPCELL8_BBNVC7     = 0x07,
        BBPCELL9_BBNVC8     = 0x08,
        BBPCELL10_BBNVC9    = 0x09,
        BBPCELL11_BBNVC10   = 0x0A,
        BBPCELL12_BBNVC11   = 0x0B,
        //BBPVC12_BBNCell12 = 0x0C,
        RESET_VALUE         = 0x00
    };
}