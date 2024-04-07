#include <IO/UART/CRXPort.h>
#include <IO/DMA/Interrupt.h>

using namespace UART;

CRXPort::CRXPort(UART_HandleTypeDef &port, const char *rxName) :
        BasePort(port),
        rxTask({Functional::Delegate<void()>::bind<&CRXPort::runRxTask>(*this), rxName, RTOS::PriorityNormal}),
        queue(rxName) {}

void CRXPort::handleInput(bool idle) {
    uint16_t currentRxPosition = INPUT_BUFFER_SIZE - port->hdmarx->Instance->CNDTR;
    
    if (currentRxPosition == lastRxPosition) {
        if (idle) queue.sendFromISR({idle, lastRxPosition, lastRxPosition});
        return;
    }
    
    // Check whether unprocessed data has been overwritten
    if (lastRxPosition > currentRxPosition) {
        if (currentRxPosition > nextToProcessPosition || nextToProcessPosition > lastRxPosition) Debug::warning();
    } else {
        if (nextToProcessPosition > lastRxPosition && nextToProcessPosition < currentRxPosition) Debug::warning();
    }
    
    uint16_t start = lastRxPosition;
    lastRxPosition = currentRxPosition;
    if (!queue.sendFromISR({idle, start, currentRxPosition})) {
        Debug::warning(); // Queue full
    }
}

[[noreturn]] void CRXPort::runRxTask() {
    InputInfoPacket packet{};
    
    __HAL_UART_ENABLE_IT(port, UART_IT_IDLE);
    DMA::storeInterruptData<BasePort>(port->hdmarx, &UART::activePort, this);
    if (HAL_UART_Receive_DMA(port, dmaBuffer, INPUT_BUFFER_SIZE) != HAL_OK)
        Debug::exception();
    
    for (;;) {
        queue.receive(&packet);
        
         if (packet.start == packet.end) {
            if (packet.idle) dataHandler(nullptr, 0, packet.idle);
            continue;
        }
        
        if (packet.start > packet.end) { // For (expected) buffer overflows
            // Only send idle flag if no additional packet gets sent
            dataHandler(&dmaBuffer[packet.start], INPUT_BUFFER_SIZE - packet.start, packet.idle && packet.end == 0);
            packet.start = 0;
        }
    
        if (packet.start != packet.end) {
            dataHandler(&dmaBuffer[packet.start], packet.end - packet.start, packet.idle);
        }
        nextToProcessPosition = packet.end;
    }
}

void UART::handleIRQ(BasePort *port) {
    if (port != nullptr && RESET != __HAL_UART_GET_FLAG(port->port, UART_FLAG_IDLE)) {
        __HAL_UART_CLEAR_IDLEFLAG(port->port);
        if (auto crxPort = dynamic_cast<CRXPort *>(port); crxPort != nullptr) {
            crxPort->handleInput(true);
        }
        
        port->interruptHandler(CallbackType::IDLE);
    }
}

/**
 * Redirects a callback to the right Port instance.
 *
 * @param type the type of callback that should get triggered
 */
void UART::handleInterrupt(CallbackType type) {
    auto port = activePort; // Race conditions should be impossible, this is just a precaution
    
    if (port != nullptr) {
        if (type & (RX_COMPLETE | RX_HALF_COMPLETE | IDLE)) {
            if (auto crxPort = dynamic_cast<CRXPort *>(port); crxPort != nullptr) {
                crxPort->handleInput(type == IDLE);
            }
        }
        
        port->interruptHandler(type);
    }
}

void CRXPort::init() {
    rxTask.init();
    queue.init();
}

void CRXPort::setDataHandler(const DataHandler &newDataHandler) {
    dataHandler = newDataHandler;
}
