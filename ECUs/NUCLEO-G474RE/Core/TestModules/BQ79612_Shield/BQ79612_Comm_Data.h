#pragma once

namespace BQ79612 {
    enum TransactionContent {
        TC_INIT,
        TC_DEV_ADR,
        TC_REG_ADR,
        TC_DATA,
        TC_CRC
    };

    enum ReqType : uint8_t {
        SINGLE_DEVICE_READ = 0,
        SINGLE_DEVICE_WRITE = 1,
        STACK_READ = 2,
        STACK_WRITE = 3,
        BROADCAST_READ = 4,
        BROADCAST_WRITE = 5,
        BROADCAST_WRITE_REVERSE = 6
    };

    enum Ping_us : uint16_t {
        WAKE = 2250,
        SHUTDOWN = 8500,
        SLEEP_TO_ACTIVE = 275,
        HARDWARE_RESET = 36000
    };

    enum Device : uint8_t {
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
        STACK_DEVICE63 = 63,
        MULTIPLE_DEVICES = 0xFF
    };

    // TODO assign proper status codes
    enum Status : uint8_t {
        ERROR = 0,
        COMPLETED = 1
    };

}
