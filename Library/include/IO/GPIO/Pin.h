#pragma once

#include <IO/GPIO/Interrupt.h>
#include <functional/Delegate.h>
#include <util.h>

namespace GPIO {
  typedef Functional::Delegate<void()> Handler;
  
  /**
   * A wrapper for HAL GPIO pins, just makes it a little easier and object oriented.
   * Due to its implementation it can be freely copied as it just a container for two HAL values.
   */
  class Pin {
    public:
      /**
       * @param port The GPIO port, you can get this from your main.h if configured in CubeMX
       * @param pin The GPIO pin number, you can get this from your main.h if configured in CubeMX
       */
      constexpr Pin(GPIO_TypeDef *port, uint16_t pin) : port(port), pin(pin) {}
      
      /**
       * Writes a GPIO value to this pin.
       *
       * @param state The new state of this pin
       */
      void write(bool state);
      
      /**
       * Toggles the state of this GPIO pin.
       */
      void toggle();
      
      /**
       * Reads the state of this GPIO pin.
       *
       * @return the state of this pin
       */
      bool read();

      /**
       * Returns the related port of this GPIO pin.
       *
       * @return the related port of this pin
       */
      GPIO_TypeDef *getPort();

      /**
       * Returns the pin initialization number.
       *
       * @return the pin init number
       */
      uint16_t getPin();
    
    private:
      GPIO_TypeDef *port;
      uint16_t pin;
  };
  
  class EXTIPin : public Pin {
      Handler handler;
    
      [[maybe_unused]]
      EXTIPin(GPIO_TypeDef *port, uint16_t pin, const Handler &handler);
    
      [[maybe_unused]]
      EXTIPin(GPIO_TypeDef *port, uint16_t pin);
    public:
      template<uint16_t gPin>
      static EXTIPin &create(GPIO_TypeDef *port) {
          auto pin = new EXTIPin(port, gPin);
          linePin<gPin> = pin;
          return *pin;
      }
    
      template<uint16_t gPin>
      static EXTIPin &create(GPIO_TypeDef *port, const Handler &handler) {
          auto pin = new EXTIPin(port, gPin, handler);
          linePin<gPin> = pin;
          return *pin;
      }
    
      static force_inline void onInterrupt();
      
      void setInterruptHandler(const Handler& newHandler);
  };
}
