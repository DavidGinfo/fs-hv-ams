#pragma once

#include "util/Common.h"

namespace BQ79612 {
    enum class TransactionContent : uint8_t {
        TC_INIT,
        TC_DEV_ADR,
        TC_REG_ADR,
        TC_DATA,
        TC_CRC
    };

    enum class ReqType : uint8_t {
        SINGLE_DEVICE_READ = 0,
        SINGLE_DEVICE_WRITE = 1,
        STACK_READ = 2,
        STACK_WRITE = 3,
        BROADCAST_READ = 4,
        BROADCAST_WRITE = 5,
        BROADCAST_WRITE_REVERSE = 6
    };

    enum class Ping_us : uint16_t {
        WAKE = 2250,
        SHUTDOWN = 8500,
        SLEEP_TO_ACTIVE = 275,
        HARDWARE_RESET = 36000
    };

    enum class Device : uint8_t {
        BASE_DEVICE = 0,
        STACK_DEVICE1 = 1,
        STACK_DEVICE2 = 2,
        STACK_DEVICE3 = 3,
        STACK_DEVICE4 = 4,
        STACK_DEVICE5 = 5,
        STACK_DEVICE6 = 6,
        STACK_DEVICE7 = 7,
        STACK_DEVICE8 = 8,
        STACK_DEVICE9 = 9,
        STACK_DEVICE10 = 10,
        STACK_DEVICE11 = 11,
        STACK_DEVICE12 = 12,
        STACK_DEVICE13 = 13,
        STACK_DEVICE14 = 14,
        STACK_DEVICE15 = 15,
        STACK_DEVICE16 = 16,
        STACK_DEVICE17 = 17,
        STACK_DEVICE18 = 18,
        STACK_DEVICE19 = 19,
        STACK_DEVICE20 = 20,
        STACK_DEVICE21 = 21,
        STACK_DEVICE22 = 22,
        STACK_DEVICE23 = 23,
        STACK_DEVICE24 = 24,
        STACK_DEVICE25 = 25,
        STACK_DEVICE26 = 26,
        STACK_DEVICE27 = 27,
        STACK_DEVICE28 = 28,
        STACK_DEVICE29 = 29,
        STACK_DEVICE30 = 30,
        STACK_DEVICE31 = 31,
        STACK_DEVICE32 = 32,
        STACK_DEVICE33 = 33,
        STACK_DEVICE34 = 34,
        STACK_DEVICE35 = 35,
        STACK_DEVICE36 = 36,
        STACK_DEVICE37 = 37,
        STACK_DEVICE38 = 38,
        STACK_DEVICE39 = 39,
        STACK_DEVICE40 = 40,
        STACK_DEVICE41 = 41,
        STACK_DEVICE42 = 42,
        STACK_DEVICE43 = 43,
        STACK_DEVICE44 = 44,
        STACK_DEVICE45 = 45,
        STACK_DEVICE46 = 46,
        STACK_DEVICE47 = 47,
        STACK_DEVICE48 = 48,
        STACK_DEVICE49 = 49,
        STACK_DEVICE50 = 50,
        STACK_DEVICE51 = 51,
        STACK_DEVICE52 = 52,
        STACK_DEVICE53 = 53,
        STACK_DEVICE54 = 54,
        STACK_DEVICE55 = 55,
        STACK_DEVICE56 = 56,
        STACK_DEVICE57 = 57,
        STACK_DEVICE58 = 58,
        STACK_DEVICE59 = 59,
        STACK_DEVICE60 = 60,
        STACK_DEVICE61 = 61,
        STACK_DEVICE62 = 62,
        STACK_DEVICE63 = 63,
        MULTIPLE_DEVICES = 0xFF
    };

    // TODO assign proper status codes
    enum Status : uint16_t {
        ERROR = 0,
        COMPLETED = 1,
    };
}