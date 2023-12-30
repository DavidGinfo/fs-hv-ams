#pragma once

#include <cstdint>
#include <util.h>

namespace Checksum {
  template<class A, typename V>
  class CRCValue {
      friend A;
      V value;
      const A &algorithm;
      
      CRCValue(V value, const A &algorithm) : value(value), algorithm(algorithm) {}
    
    public:
      void update(uint8_t data);
      
      void update(uint16_t size, uint8_t *data);

      void calc(uint16_t size, uint8_t *data);
      
      void reset();
      
      [[nodiscard]] V get() const;
  };
  
  class CRC8Algorithm;
  class CRC16Algorithm;

  typedef CRCValue<CRC8Algorithm, uint8_t> CRC8Value;
  typedef CRCValue<CRC16Algorithm, uint16_t> CRC16Value;

  class CRC8Algorithm {
      uint8_t polynomial;
      uint8_t start;
      uint8_t xorOut;
    
    public:
      consteval explicit CRC8Algorithm(uint8_t polynomial, uint8_t start = 0, uint8_t xorOut = 0) :
              polynomial(polynomial), start(start), xorOut(xorOut) {}
      
      [[nodiscard]] uint8_t getStart() const;
      
      [[nodiscard]] CRC8Value create() const;
      
      [[nodiscard]] uint8_t update(uint8_t crc, uint8_t data) const;
      
      [[nodiscard]] uint8_t update(uint8_t crc, uint16_t size, uint8_t *data) const;
      
      [[nodiscard]] uint8_t complete(uint8_t crc) const;
      
      [[nodiscard]] uint8_t calc(uint16_t size, uint8_t *data) const;
  };

  class CRC16Algorithm {
      uint16_t polynomial;
      uint16_t start;
      uint16_t xorOut;

    public:
      consteval explicit CRC16Algorithm(uint16_t polynomial, uint16_t start = 0, uint16_t xorOut = 0) :
                                                                                                    polynomial(polynomial), start(start), xorOut(xorOut) {}

      [[nodiscard]] uint16_t getStart() const;

      [[nodiscard]] CRC16Value create() const;

      [[nodiscard]] uint16_t update(uint16_t crc, uint8_t data) const;

      [[nodiscard]] uint16_t update(uint16_t crc, uint16_t size, uint8_t *data) const;

      [[nodiscard]] uint16_t complete(uint16_t crc) const;

      [[nodiscard]] uint16_t calc(uint16_t size, uint8_t *data) const;
  };

  inline constexpr CRC8Algorithm SMBUS(0x07, 0x00, 0x00);
  inline constexpr CRC8Algorithm MAXIM_BMS(reverse(0xB2), 0x00, 0x00);
  inline constexpr CRC16Algorithm IBM_AMS(reverse16(0x8005), 0xFFFF, 0x0000);
  
  // TODO Check again when gcc constexpr evaluation does not fail for small loops
  /*
  struct ACRC8Algorithm : public CRC8Algorithm {
      uint8_t table[256] = {0};

      consteval explicit ACRC8Algorithm(uint8_t polynomial, uint8_t start = 0, uint8_t xorOut = 0) :
              CRC8Algorithm(polynomial, start, xorOut) {
          for (uint8_t index = 0; index < 30; index++) {
              uint8_t crc = index;

              for (uint8_t i = 0; i < 8; i++) {
                  if (crc & 0x80)
                      crc = (crc << 1) ^ polynomial;
                  else
                      crc = (crc << 1);
              }

              table[index] = crc;
          }
      }
  };

  template<ACRC8Algorithm algorithm>
  uint8_t updateCRC8(uint8_t crc, uint16_t size, uint8_t *data) {
      while (size--) {
          crc = crc ^ *data++; // Apply Byte

          crc = algorithm.table[crc & 0xFF]; // One round of 8-bits
      }

      return crc;
  }
   */
}
