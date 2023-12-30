#include <IO/GPIO/Pin.h>
#include <IO/GPIO/Interrupt.h>

using namespace GPIO;

void Pin::write(bool state) {
    HAL_GPIO_WritePin(port, pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void Pin::toggle() {
    HAL_GPIO_TogglePin(port, pin);
}

bool Pin::read() {
    return HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_SET;
}

GPIO_TypeDef *Pin::getPort(){
    return port;
}

uint16_t Pin::getPin(){
    return pin;
}

EXTIPin::EXTIPin(GPIO_TypeDef *port, uint16_t pin, const Handler &handler) : Pin(port, pin), handler(handler) {}

EXTIPin::EXTIPin(GPIO_TypeDef *port, uint16_t pin) : Pin(port, pin), handler() {}

void EXTIPin::setInterruptHandler(const Handler& newHandler) {
    handler = newHandler;
}

void EXTIPin::onInterrupt() {
    EXTIPin *pin = activePin;
    
    if (pin != nullptr) {
        pin->handler();
    }
}

void HAL_GPIO_EXTI_Callback([[maybe_unused]] uint16_t pin) {
    EXTIPin::onInterrupt();
}
