#include <IO/UART/Port.h>
#include <IO/DMA/Interrupt.h>

using namespace UART;
using namespace Parallel;


BasePort::BasePort(UART_HandleTypeDef &port): port(&port) {}

[[maybe_unused]]
bool BasePort::transmitSync(TransferType *data, TransferSize size, TimeMS timeout) const {
    return HAL_UART_Transmit(port, data, size, timeout) == HAL_OK;
}

[[maybe_unused]]
bool BasePort::transmitIT(TransferType *data, TransferSize size) const {
    return HAL_UART_Transmit_IT(port, data, size) == HAL_OK;
}

[[maybe_unused]]
void BasePort::setInterruptHandler(const InterruptHandler& newInterruptHandler) {
    interruptHandler = newInterruptHandler;
}

[[maybe_unused]]
bool BasePort::transmitDMA(TransferType *data, TransferSize size) {
    DMA::storeInterruptData<BasePort>(port->hdmatx, &UART::activePort, this);
    return HAL_UART_Transmit_DMA(port, data, size) == HAL_OK;
}

Port::Port(UART_HandleTypeDef &port) : BasePort(port) {}

[[maybe_unused]]
bool Port::receiveSync(TransferType *data, TransferSize size, TimeMS timeout) const {
    return HAL_UART_Receive(port, data, size, timeout) == HAL_OK;
}

[[maybe_unused]]
bool Port::receiveIT(TransferType *data, TransferSize size) const {
    return HAL_UART_Receive_IT(port, data, size) == HAL_OK;
}

[[maybe_unused]]
bool Port::receiveDMA(TransferType *data, TransferSize size) {
    DMA::storeInterruptData<BasePort>(port->hdmarx, &UART::activePort, this);
    return HAL_UART_Receive_DMA(port, data, size) == HAL_OK;
}

void HAL_UART_TxCpltCallback([[maybe_unused]] UART_HandleTypeDef *handle) {
    handleInterrupt(CallbackType::TX_COMPLETE);
}

void HAL_UART_RxCpltCallback([[maybe_unused]] UART_HandleTypeDef *handle) {
    handleInterrupt(CallbackType::RX_COMPLETE);
}

void HAL_UART_TxHalfCpltCallback([[maybe_unused]] UART_HandleTypeDef *handle) {
    handleInterrupt(CallbackType::TX_HALF_COMPLETE);
}

void HAL_UART_RxHalfCpltCallback([[maybe_unused]] UART_HandleTypeDef *handle) {
    handleInterrupt(CallbackType::RX_HALF_COMPLETE);
}

void HAL_UART_ErrorCallback([[maybe_unused]] UART_HandleTypeDef *handle) {
    handleInterrupt(CallbackType::ERROR);
}

void HAL_UART_AbortCpltCallback([[maybe_unused]] UART_HandleTypeDef *handle) {
    handleInterrupt(CallbackType::ABORT);
}
