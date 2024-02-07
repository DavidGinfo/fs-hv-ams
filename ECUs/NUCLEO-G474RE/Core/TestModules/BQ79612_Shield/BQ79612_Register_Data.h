#pragma once

#include <util/Common.h>

namespace BQ79612 {
    enum ReadWriteRegister : uint16_t {
        OTP_ECC_TEST = 0x034C,
        Control1     = 0x0309,
        DIR0_ADDR    = 0x0306,
        COMM_CTRL    = 0x0308
    };

    enum ReadOnlyRegister : uint16_t
    {
        PART_ID       = 0x0500,
        FAULT_SUMMERY = 0x052D
    };

    enum OTPRegister : uint16_t
    {

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

    enum COMM_CTRL : uint8_t {
        STACK_DEV = 1 << 1,
        TOP_STACK = 1
    };
}