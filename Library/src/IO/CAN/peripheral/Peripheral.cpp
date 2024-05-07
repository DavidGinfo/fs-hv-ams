#include <IO/CAN/Peripheral.h>
#include <IO/CAN/Bus.h>
#include <IO/CAN/Config.h>


using namespace CAN;

#if FD_CAN

#if FEATURE_CAN_PERIPHERAL
extern FDCAN_HandleTypeDef hfdcan;
CAN::Peripheral CAN::peripheral0(&hfdcan, 0);
extern const uint32_t CAN::PERIPHERAL_0_PRIORITY __attribute((weak)) = DEFAULT_IRQ_PREEMPTION_PRIORITY;
#endif
#if FEATURE_CAN1_PERIPHERAL
extern FDCAN_HandleTypeDef hfdcan1;
CAN::Peripheral CAN::peripheral1(&hfdcan1, 0);
extern const uint32_t CAN::PERIPHERAL_1_PRIORITY __attribute((weak)) = DEFAULT_IRQ_PREEMPTION_PRIORITY;
#endif
#if FEATURE_CAN2_PERIPHERAL
extern FDCAN_HandleTypeDef hfdcan2;
CAN::Peripheral CAN::peripheral2(&hfdcan2, 0);
extern const uint32_t CAN::PERIPHERAL_2_PRIORITY __attribute((weak)) = DEFAULT_IRQ_PREEMPTION_PRIORITY;
#endif
#if FEATURE_CAN3_PERIPHERAL
extern FDCAN_HandleTypeDef hfdcan3;
CAN::Peripheral CAN::peripheral3(&hfdcan3, 0);
extern const uint32_t CAN::PERIPHERAL_3_PRIORITY __attribute((weak)) = DEFAULT_IRQ_PREEMPTION_PRIORITY;
#endif

void CAN::initPeripherals() {
    #if FEATURE_CAN_PERIPHERAL
    if (peripheral0.getBus() == INVALID_BUS_INDEX) {
        Debug::exception(); // An activated peripheral should be mapped to a bus
    }
    
    if (!buses[peripheral0.getBus()]->isValid() || buses[peripheral0.getBus()] == nullptr) {
        Debug::exception(); // Peripheral should be mapped correctly and should have an associated manager
    }
    
    HAL_FDCAN_ConfigInterruptLines(&hfdcan, INTERRUPT_GROUPS, FDCAN_INTERRUPT_LINE0);
    
    // enable external interrupts and set their priorities
    HAL_NVIC_SetPriority(FDCAN_IT0_IRQn, PERIPHERAL_0_PRIORITY, IRQ_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(FDCAN_IT0_IRQn);
    
    // activate notifications (BUFFER_INDEXES should be ignored)
    HAL_FDCAN_ActivateNotification(&hfdcan, ACTIVE_NOTIFICATIONS, BUFFER_INDEXES);
    #endif
    
    #if FEATURE_CAN1_PERIPHERAL
    if (peripheral1.getBus() == INVALID_BUS_INDEX) {
        Debug::exception(); // An activated peripheral should be mapped to a bus
    }
    
    if (!buses[peripheral1.getBus()]->isValid() || buses[peripheral1.getBus()] == nullptr) {
        Debug::exception(); // Peripheral should be mapped correctly and should have an associated manager
    }
    
    HAL_FDCAN_ConfigInterruptLines(&hfdcan1, INTERRUPT_GROUPS, FDCAN_INTERRUPT_LINE0);
    
    // enable external interrupts and set their priorities
    HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, PERIPHERAL_1_PRIORITY, IRQ_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
    
    // activate notifications (BUFFER_INDEXES should be ignored)
    HAL_FDCAN_ActivateNotification(&hfdcan1, ACTIVE_NOTIFICATIONS, BUFFER_INDEXES);
    #endif
    
    #if FEATURE_CAN2_PERIPHERAL
    if (peripheral2.getBus() == INVALID_BUS_INDEX) {
        Debug::exception(); // An activated peripheral should be mapped to a bus
    }
    
    if (!buses[peripheral2.getBus()]->isValid() || buses[peripheral2.getBus()] == nullptr) {
        Debug::exception(); // Peripheral should be mapped correctly and should have an associated manager
    }
    
    // activate interrupts for interrupt line 0
    HAL_FDCAN_ConfigInterruptLines(&hfdcan2, INTERRUPT_GROUPS, FDCAN_INTERRUPT_LINE0);
    
    // enable external interrupts and set their priorities
    HAL_NVIC_SetPriority(FDCAN2_IT0_IRQn, PERIPHERAL_2_PRIORITY, IRQ_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(FDCAN2_IT0_IRQn);
    
    // activate notifications
    HAL_FDCAN_ActivateNotification(&hfdcan2, ACTIVE_NOTIFICATIONS, BUFFER_INDEXES);
    #endif
    
#if FEATURE_CAN3_PERIPHERAL
    if (peripheral3.getBus() == INVALID_BUS_INDEX) {
        Debug::exception(); // An activated peripheral should be mapped to a bus
    }
    
    if (!buses[peripheral3.getBus()]->isValid() || buses[peripheral3.getBus()] == nullptr) {
        Debug::exception(); // Peripheral should be mapped correctly and should have an associated manager
    }
    
    // activate interrupts for interrupt line 0
    HAL_FDCAN_ConfigInterruptLines(&hfdcan3, INTERRUPT_GROUPS, FDCAN_INTERRUPT_LINE0);
    
    // enable external interrupts and set their priorities
    HAL_NVIC_SetPriority(FDCAN3_IT0_IRQn, PERIPHERAL_3_PRIORITY, IRQ_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(FDCAN3_IT0_IRQn);
    
    // activate notifications
    HAL_FDCAN_ActivateNotification(&hfdcan3, ACTIVE_NOTIFICATIONS, BUFFER_INDEXES);
#endif
}


Peripheral::Peripheral(FDCAN_HandleTypeDef *const handle, uint8_t firstFilterIndex) :
        handle(handle),
        firstFilterIndex(firstFilterIndex),
        mappedBus(INVALID_BUS_INDEX) {}

void Peripheral::mapBus(uint8_t bus) {
    if (bus >= BUS_COUNT) {
        Debug::exception(); // Out of bounds
    }
    
    mappedBus = bus;
}

uint8_t Peripheral::getBus() const {
    return mappedBus;
}


#if FEATURE_CAN_PERIPHERAL
extern "C" void FDCAN_IT0_IRQHandler() {
    auto previous = activeInterruptIndex;
    activeInterruptIndex = peripheral0.getBus();
    HAL_FDCAN_IRQHandler(&hfdcan);
    activeInterruptIndex = previous;
}
#endif

#if FEATURE_CAN1_PERIPHERAL
extern "C" void FDCAN1_IT0_IRQHandler() {
    auto previous = activeInterruptIndex;
    activeInterruptIndex = peripheral1.getBus();
    HAL_FDCAN_IRQHandler(&hfdcan1);
    activeInterruptIndex = previous;
}
#endif

#if FEATURE_CAN2_PERIPHERAL
extern "C" void FDCAN2_IT0_IRQHandler() {
    auto previous = activeInterruptIndex;
    activeInterruptIndex = peripheral2.getBus();
    HAL_FDCAN_IRQHandler(&hfdcan2);
    activeInterruptIndex = previous;
}
#endif

#if FEATURE_CAN3_PERIPHERAL
extern "C" void FDCAN3_IT0_IRQHandler() {
    auto previous = activeInterruptIndex;
    activeInterruptIndex = peripheral3.getBus();
    HAL_FDCAN_IRQHandler(&hfdcan3);
    activeInterruptIndex = previous;
}
#endif

#endif

#if !FD_CAN

#if FEATURE_CAN_PERIPHERAL
extern CAN_HandleTypeDef hcan;
CAN::Peripheral CAN::peripheral0(&hcan, 0);
extern const uint32_t CAN::PERIPHERAL_0_PRIORITY __attribute((weak)) = DEFAULT_IRQ_PREEMPTION_PRIORITY;
#endif
#if FEATURE_CAN1_PERIPHERAL
extern CAN_HandleTypeDef hcan1;
CAN::Peripheral CAN::peripheral1(&hcan1, 0);
extern const uint32_t CAN::PERIPHERAL_1_PRIORITY __attribute((weak)) = DEFAULT_IRQ_PREEMPTION_PRIORITY;
#endif
#if FEATURE_CAN2_PERIPHERAL
extern CAN_HandleTypeDef hcan2;
CAN::Peripheral CAN::peripheral2(&hcan2, CAN::FIRST_SLAVE_FILTER_BANK);
extern const uint32_t CAN::PERIPHERAL_2_PRIORITY __attribute((weak)) = DEFAULT_IRQ_PREEMPTION_PRIORITY;
#endif

void CAN::initPeripherals() {
    #if FEATURE_CAN_PERIPHERAL
    if (peripheral0.getBus() == INVALID_BUS_INDEX) {
        Debug::exception(); // An activated peripheral should be mapped to a bus
    }
    
    if (!buses[peripheral0.getBus()]->isValid() || buses[peripheral0.getBus()] == nullptr) {
        Debug::exception(); // Peripheral should be mapped correctly and should have an associated manager
    }
    
    HAL_NVIC_SetPriority(CAN_TX_IRQn, PERIPHERAL_0_PRIORITY, IRQ_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(CAN_TX_IRQn);
    HAL_NVIC_SetPriority(CAN_RX0_IRQn, PERIPHERAL_0_PRIORITY, IRQ_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(CAN_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN_SCE_IRQn, PERIPHERAL_0_PRIORITY, IRQ_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(CAN_SCE_IRQn);
    
    HAL_CAN_ActivateNotification(&hcan, ACTIVE_NOTIFICATIONS);
    #endif
    
    #if FEATURE_CAN1_PERIPHERAL
    if (peripheral1.getBus() == INVALID_BUS_INDEX) {
        Debug::exception(); // An activated peripheral should be mapped to a bus
    }
    
    if (!buses[peripheral1.getBus()]->isValid() || buses[peripheral1.getBus()] == nullptr) {
        Debug::exception(); // Peripheral should be mapped correctly and should have an associated manager
    }
    
    HAL_NVIC_SetPriority(CAN1_TX_IRQn, PERIPHERAL_1_PRIORITY, IRQ_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, PERIPHERAL_1_PRIORITY, IRQ_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN1_SCE_IRQn, PERIPHERAL_1_PRIORITY, IRQ_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);
    
    HAL_CAN_ActivateNotification(&hcan1, ACTIVE_NOTIFICATIONS);
    #endif
    
    #if FEATURE_CAN2_PERIPHERAL
    if (peripheral2.getBus() == INVALID_BUS_INDEX) {
        Debug::exception(); // An activated peripheral should be mapped to a bus
    }
    
    if (!buses[peripheral2.getBus()]->isValid() || buses[peripheral2.getBus()] == nullptr) {
        Debug::exception(); // Peripheral should be mapped correctly and should have an associated manager
    }
    
    HAL_NVIC_SetPriority(CAN2_TX_IRQn, PERIPHERAL_2_PRIORITY, IRQ_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(CAN2_TX_IRQn);
    HAL_NVIC_SetPriority(CAN2_RX0_IRQn, PERIPHERAL_2_PRIORITY, IRQ_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN2_SCE_IRQn, PERIPHERAL_2_PRIORITY, IRQ_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(CAN2_SCE_IRQn);
    
    HAL_CAN_ActivateNotification(&hcan2, ACTIVE_NOTIFICATIONS);
    #endif
}


Peripheral::Peripheral(CAN_HandleTypeDef *const handle, uint8_t firstFilterIndex) :
        handle(handle),
        firstFilterIndex(firstFilterIndex),
        mappedBus(INVALID_BUS_INDEX) {}

void Peripheral::mapBus(uint8_t bus) {
    if (bus >= BUS_COUNT) {
        Debug::exception(); // Out of bounds
    }
    
    mappedBus = bus;
}

uint8_t Peripheral::getBus() const {
    return mappedBus;
}


#if FEATURE_CAN_PERIPHERAL
extern "C" void CAN_TX_IRQHandler() {
    auto previous = activeInterruptIndex;
    activeInterruptIndex = peripheral0.getBus();
    HAL_CAN_IRQHandler(&hcan);
    activeInterruptIndex = previous;
}

extern "C" void CAN_RX0_IRQHandler() {
    auto previous = activeInterruptIndex;
    activeInterruptIndex = peripheral0.getBus();
    HAL_CAN_IRQHandler(&hcan);
    activeInterruptIndex = previous;
}

extern "C" void CAN_SCE_IRQHandler() {
    auto previous = activeInterruptIndex;
    activeInterruptIndex = peripheral0.getBus();
    HAL_CAN_IRQHandler(&hcan);
    activeInterruptIndex = previous;
}
#endif

#if FEATURE_CAN1_PERIPHERAL
extern "C" void CAN1_TX_IRQHandler() {
    auto previous = activeInterruptIndex;
    activeInterruptIndex = peripheral1.getBus();
    HAL_CAN_IRQHandler(&hcan1);
    activeInterruptIndex = previous;
}

extern "C" void CAN1_RX0_IRQHandler() {
    auto previous = activeInterruptIndex;
    activeInterruptIndex = peripheral1.getBus();
    HAL_CAN_IRQHandler(&hcan1);
    activeInterruptIndex = previous;
}

extern "C" void CAN1_SCE_IRQHandler() {
    auto previous = activeInterruptIndex;
    activeInterruptIndex = peripheral1.getBus();
    HAL_CAN_IRQHandler(&hcan1);
    activeInterruptIndex = previous;
}
#endif

#if FEATURE_CAN2_PERIPHERAL
extern "C" void CAN2_TX_IRQHandler() {
    auto previous = activeInterruptIndex;
    activeInterruptIndex = peripheral2.getBus();
    HAL_CAN_IRQHandler(&hcan2);
    activeInterruptIndex = previous;
}

extern "C" void CAN2_RX0_IRQHandler() {
    auto previous = activeInterruptIndex;
    activeInterruptIndex = peripheral2.getBus();
    HAL_CAN_IRQHandler(&hcan2);
    activeInterruptIndex = previous;
}

extern "C" void CAN2_SCE_IRQHandler() {
    auto previous = activeInterruptIndex;
    activeInterruptIndex = peripheral2.getBus();
    HAL_CAN_IRQHandler(&hcan2);
    activeInterruptIndex = previous;
}
#endif

#endif