#include <data/checksum/CRC.h>
#include "CRCValue.cpp"

using namespace Checksum;

template void CRC16Value::update(uint8_t data);

template void CRC16Value::update(uint16_t size, uint8_t *data);

template uint16_t CRC16Value::get() const;

template void CRC16Value::reset();

CRC16Value CRC16Algorithm::create() const {
    return {start, *this};
}

uint16_t CRC16Algorithm::getStart() const {
    return start;
}

uint16_t CRC16Algorithm::update(uint16_t crc, uint8_t data) const {
    crc = crc ^ static_cast<uint16_t>(data);

    for (uint8_t i = 0; i < 8; i++) {
        if (crc & 1)
            crc = (crc >> 1) ^ polynomial;
        else
            crc = (crc >> 1);
    }

    return crc;
}

uint16_t CRC16Algorithm::update(uint16_t crc, uint16_t size, uint8_t *data) const {
    while (size--) {
        crc = update(crc, *data++);
    }

    return crc;
}

uint16_t CRC16Algorithm::complete(uint16_t crc) const {
    return crc ^ xorOut;
}

uint16_t CRC16Algorithm::calc(uint16_t size, uint8_t *data) const {
    return update(start, size, data) ^ xorOut;
}