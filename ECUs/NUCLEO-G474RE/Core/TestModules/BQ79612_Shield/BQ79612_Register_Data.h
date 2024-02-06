#pragma once

#include <util/Common.h>

namespace BQ79612 {

    // TODO implement
    enum ReadWriteRegister : uint16_t {

    };

    enum ReadOnlyRegister : uint16_t {

    };

    enum OTPRegister : uint16_t {

    };

    enum ReadRegister : uint16_t {
        OTP_ECC_TEST_R = 0x034C,
        Control1_R     = 0x0309,
        DIR0_ADDR_R    = 0x0306,
        COMM_CTRL_R    = 0x0308,
        PART_ID        = 0x0500,
        FAULT_SUMMERY  = 0x052D
    };

    enum WriteRegister : uint16_t {
        OTP_ECC_TEST_W = 0x034C,
        Control1_W     = 0x0309,
        DIR0_ADDR_W    = 0x0306,
        COMM_CTRL_W    = 0x0308
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