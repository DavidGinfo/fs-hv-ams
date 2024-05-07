#include <IO/EEPROM/EEPROM.h>
#include <parallel/Task.hpp>

using namespace eeprom;

uint16_t EEPROM::bytesToWrite(uint16_t size, uint16_t offset) {
    // Calculate number of bytes to write
    uint16_t bytesRemaining = pageSize - offset;
    if (size < bytesRemaining)
    {
        return size;
    }
    else
    {
        return bytesRemaining;
    }
}

bool EEPROM::read(uint16_t page, uint16_t offset, uint8_t *data, uint16_t size) {
    // Write data to EEPROM
    int paddrposition = log(pageSize)/log(2);

    uint16_t startPage = page;
    uint16_t endPage = page + ((size+offset)/pageSize);

    uint16_t numofpages = (endPage-startPage) + 1;
    uint16_t pos=0;

    for (int i=0; i<numofpages; i++)
    {
        uint16_t MemAddress = startPage<<paddrposition | offset;
        uint16_t bytesremaining = bytesToWrite(size, offset);
        if (HAL_I2C_Mem_Read(handleTypeDef, address << 1, MemAddress, memAddSize, &data[pos], bytesremaining, timeout) != HAL_OK) {
            return false;
        }
        startPage += 1;
        offset=0;
        size = size-bytesremaining;
        pos += bytesremaining;
    }
    return true;
}

bool EEPROM::write(uint16_t page, uint16_t offset, uint8_t *data, uint16_t size) {
    // Find out the number of bit, where the page addressing starts

    int paddrposition = log(pageSize)/log(2);

    // calculate the start page and the end page
    uint16_t startPage = page;
    uint16_t endPage = page + ((size+offset)/pageSize);

    // number of pages to be written
    uint16_t numofpages = (endPage-startPage) + 1;
    uint16_t pos=0;

    // Pull write enable pin to ground to enable write operation
    writeControlPin.write(0);

    // write the data
    for (int i=0; i<numofpages; i++)
    {
        /* calculate the address of the memory location
		 * Here we add the page address with the byte address
		 */
        uint16_t MemAddress = startPage<<paddrposition | offset;
        uint16_t bytesremaining = bytesToWrite(size, offset);  // calculate the remaining bytes to be written

        if (HAL_I2C_Mem_Write(handleTypeDef, address << 1, MemAddress, memAddSize, &data[pos], bytesremaining, timeout) != HAL_OK) { // write the data to the EEPROM
            // Pull write enable pin high to disable write operation
            writeControlPin.write(1);
            return false;
        }
        startPage += 1;  // increment the page, so that a new page address can be selected for further write
        offset=0;   // since we will be writing to a new page, so offset will be 0
        size = size-bytesremaining;  // reduce the size of the bytes
        pos += bytesremaining;  // update the position for the data buffer

        Parallel::delay(5);  // Write cycle delay (5ms)
    }
    // Pull write enable pin high to disable write operation
    writeControlPin.write(1);
    return true;
}