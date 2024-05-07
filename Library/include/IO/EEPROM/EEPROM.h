#pragma once

#include <IO/GPIO/Pin.h>
#include <IO/CAN/Signal.h>


namespace eeprom {
    class EEPROM {
        uint16_t address;
        uint16_t pageSize;
        GPIO::Pin writeControlPin;
        I2C_HandleTypeDef *handleTypeDef;

      private:
        uint16_t bytesToWrite(uint16_t size, uint16_t offset);
        uint16_t timeout;
        const uint16_t memAddSize = 2;


      public:
        EEPROM(uint16_t address, uint16_t pageSize, GPIO::Pin writeControlPin, I2C_HandleTypeDef *handleTypeDef, uint16_t timeout = 1000) :
                address(address), pageSize(pageSize), writeControlPin(writeControlPin), handleTypeDef(handleTypeDef), timeout(timeout) {}

        bool write(uint16_t page, uint16_t offset, uint8_t *data, uint16_t size);

        bool read(uint16_t page, uint16_t offset, uint8_t *data, uint16_t size);

    };
}