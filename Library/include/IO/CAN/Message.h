#pragma once

#include <cstdint>
#include <cstring>
#include <IO/CAN/Timer.h>
#include <functional/Delegate.h>
#include <util.h>

#include "Signal.h"

namespace CAN {

    /* Helper functions for swapping bytes for Motorola messages/signals */
    template <typename T>
    T swapByteOrder(T value) {
        T result;
        uint8_t* src = reinterpret_cast<uint8_t*>(&value);
        uint8_t* dest = reinterpret_cast<uint8_t*>(&result);

        for (size_t i = 0; i < sizeof(T); ++i) {
            dest[sizeof(T) - i - 1] = src[i];
        }

        return result;
    }

    template <typename T>
    T bigEndianToLittleEndian(T value) {
        T swapped = swapByteOrder(value);
        return swapped;
    }


  typedef Functional::Delegate<void()> Handler;
  
  inline constexpr uint8_t convertStartBigEndianToLittle(uint8_t length, uint8_t start) {
      return ((length - start - 8)/8)*8 + start % 8;
  }
  
  /**
   * A generic base class for the actual template to ease the creation of generic functions.
   *
   * @author David Schwärzle <mailto:david.schwaerzle@highspeed-karlsruhe.de>
   */
  class MessageBase {
    protected:
      uint32_t id;
      bool extended;
      uint8_t length;
      uint16_t cycleTime;
      Handler handler;
    
    public:
      MessageBase(uint32_t id,
                  bool extended,
                  uint8_t length,
                  uint16_t cycleTime) noexcept: id(id),
                                                extended(extended),
                                                length(length),
                                                cycleTime(cycleTime) {}
      
      virtual void update(const uint8_t data[], uint32_t newTimestamp) = 0;
      
      virtual uint8_t *getData(bool fetchFromSignals) = 0;
      
      uint8_t *getData() {
          return getData(true);
      }
      
      [[nodiscard]] uint32_t getId() const {
          return id;
      }
      
      [[nodiscard]] bool isExtended() const {
          return extended;
      }
      
      [[nodiscard]] uint8_t getLength() const {
          return length;
      }
      
      [[nodiscard]] uint16_t getCycleTime() const {
          return cycleTime;
      }
      
      [[nodiscard]] Handler const &getHandler() const {
          return handler;
      };
      
      /**
       * Registers a handler that is called before a message gets send / after it has been received.
       * Warning: Without FreeRTOS RX events will be executed in an ISR!
       * Warning: NEVER call this function after initialization, it is not thread safe!
       *
       * @param newHandler the handler to be registered
       */
      void setHandler(const Handler& newHandler) {
          handler = newHandler;
      }
  };
  
  template<typename T>
  class Message : public MessageBase {
    private:
      uint32_t timestamp = 0;
      T value = 0;
      
      Signal **signals;
      uint8_t signalCount;
    
    public:
      Message(uint32_t id,
              bool extended,
              uint8_t length,
              uint16_t cycleTime,
              Signal *signals[],
              uint8_t signalCount) noexcept: MessageBase(id, extended, length, cycleTime),
                                             signals(signals),
                                             signalCount(signalCount) {}
      
      void update(const uint8_t data[]) {
          memcpy(&value, data, length);

          if(signalCount == 0)
              return;

          // If big endian than swap the message itself and every byte
          if(signals[0]->isBigEndian()) {
              value = bigEndianToLittleEndian<T>(value);

          }
          
          for (uint8_t i = 0; i < signalCount; i++) {
              Signal *signal = signals[i];
              uint16_t startBit;
              if (signal->isBigEndian())
                  // if length of message is 32, startbit was 0, we have two signals with 16 bit length each,
                  // then the new start bit is 16. If startbit was 16, it is now 0.
                  // sizeof returns the size in bytes, therefore multiply by 8
                  startBit = (sizeof(value) * 8) - signal->startBit - signal->length;
              else
                  startBit = signal->startBit;
               SignalData signalData = (value >> startBit);
              signal->updateData(signalData & Data::dataMask<SignalData>(signal->length));
          }
      }
      
      void update(const uint8_t data[], uint32_t newTimestamp) override {
          timestamp = newTimestamp;
          update(data);
      }
      
      uint8_t *getData(bool fetchFromSignals) override {
          if (fetchFromSignals) {
              for (uint8_t i = 0; i < signalCount; i++) {
                  Signal *signal = signals[i];
                  
                  auto dataMask = Data::dataMask<SignalData>(signal->length);
                  // Mask data to max value again just to be certain
                  T setMask = static_cast<T>(signal->getData() & dataMask);
                  T clearMask = static_cast<T>(~signal->getData() & dataMask); // bits to clear are 1
                  
                  if (signal->isBigEndian()) {
                      uint8_t start = convertStartBigEndianToLittle(length, signal->startBit);
                      setMask = swap<T>(setMask << start);
                      clearMask = swap<T>(clearMask << start);
                  } else {
                      setMask <<= signal->startBit;
                      clearMask <<= signal->startBit;
                  }
                  
                  value |= setMask;
                  value &= ~clearMask;
              }
          }
          
          return (uint8_t *) &value;
      }
      
      bool validate(uint32_t currentTimestamp, float missingFactor) {
          return static_cast<float>(currentTimestamp - timestamp) / static_cast<float>(cycleTime) < missingFactor;
      }
    
      bool validate(float missingFactor) {
          return static_cast<float>(getTimestamp() - timestamp) / static_cast<float>(cycleTime) < missingFactor;
      }
  };
}
