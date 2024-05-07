#pragma once

#include <util/Common.h>

#if FD_CAN

namespace CAN {
    /**
   * Filters per CAN peripheral.
   */
    inline constexpr uint8_t FILTER_COUNT = 28;
  
    /**
   * Sub priority for CAN interrupts. Should be irrelevant and is thus not configurable.
   */
    inline constexpr uint32_t IRQ_SUB_PRIORITY = 0;

    /**
    * activates the following groups of interrupts for a specific interrupt line
    */

/* For some reason in STM32H7 Those do not get defined by default */
#ifndef FDCAN_IT_LIST_RX_FIFO0
#define FDCAN_IT_LIST_RX_FIFO0         (FDCAN_IT_RX_FIFO0_MESSAGE_LOST | \
                                    FDCAN_IT_RX_FIFO0_FULL         | \
                                    FDCAN_IT_RX_FIFO0_NEW_MESSAGE)
#endif

#ifndef FDCAN_IT_LIST_SMSG
#define FDCAN_IT_LIST_SMSG             (FDCAN_IT_TX_ABORT_COMPLETE | \
                                    FDCAN_IT_TX_COMPLETE | \
                                    FDCAN_IT_RX_HIGH_PRIORITY_MSG)
#endif
    inline constexpr uint32_t INTERRUPT_GROUPS = FDCAN_IT_LIST_RX_FIFO0 | FDCAN_IT_LIST_SMSG;
  
    /**
   * specifies the tx-buffers where the notifications should be available
   */
    inline constexpr uint32_t BUFFER_INDEXES = FDCAN_TX_BUFFER0 | FDCAN_TX_BUFFER1 | FDCAN_TX_BUFFER2;
  
    /**
   * Activates notifications on interrupts for empty fifo, new messages an errors.
   */
#ifndef FDCAN_IT_GROUP_SMSG
    inline constexpr uint32_t ACTIVE_NOTIFICATIONS = FDCAN_IT_TX_FIFO_EMPTY | FDCAN_IT_RX_FIFO0_NEW_MESSAGE | FDCAN_IT_ERROR_PASSIVE | FDCAN_IT_ERROR_WARNING | FDCAN_IT_BUS_OFF;
#else
    inline constexpr uint32_t ACTIVE_NOTIFICATIONS = FDCAN_IT_TX_FIFO_EMPTY | FDCAN_IT_RX_FIFO0_NEW_MESSAGE | FDCAN_IT_GROUP_SMSG;
#endif
  
    /**
   * Do not edit manually. This references the bus of an active interrupt (as it is not possible to pass this through the HAL handler).
   * May be subject to nesting, this is fully supported.
   */
    volatile inline uint8_t activeInterruptIndex;
  
    /**
   * For internal use only. Initializes the CAN peripherals.
   */
    void initPeripherals();
  
    /**
   * Representation of a physical CAN peripheral of the microcontroller.
   * This might be connected and thus mapped accordingly to a logical CAN bus (bus numbers of the vehicles).
   */
    class Peripheral {
        /**
       * The logical CAN bus that is connected to this interface.
       * Starts of a INVALID_BUS_INDEX to indicate no connection.
       */
        uint8_t mappedBus;
      public:
        /**
       * Defines the CAN peripheral this object is representing.
       */
        FDCAN_HandleTypeDef *const handle;
      
        /**
       * The first CAN filter bank that can be used with this peripheral.
       * As we use FDCAN this value should always be zero.
       */
        const uint8_t firstFilterIndex;
      
        explicit Peripheral(FDCAN_HandleTypeDef *handle, uint8_t firstFilterIndex);
      
        void mapBus(uint8_t bus);
        [[nodiscard]] uint8_t getBus() const;
    };
}

#endif

#if !FD_CAN

namespace CAN {
    /**
   * Filters per CAN peripheral.
   */
    inline constexpr uint8_t FILTER_COUNT = 14;
  
    /**
   * First filter bank that is associated with the slave CAN peripheral.
   */
    inline constexpr uint8_t FIRST_SLAVE_FILTER_BANK = 14;
  
    /**
   * Sub priority for CAN interrupts. Should be irrelevant and is thus not configurable.
   */
    inline constexpr uint32_t IRQ_SUB_PRIORITY = 0;
    /**
   * Activates notifications on interrupts for empty mailboxes, pending messages an errors.
   */
    inline constexpr uint32_t ACTIVE_NOTIFICATIONS = CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_ERROR;
  
    /**
   * Do not edit manually. This references the bus of an active interrupt (as it is not possible to pass this through the HAL handler).
   * May be subject to nesting, this is fully supported.
   */
    volatile inline uint8_t activeInterruptIndex;
  
    /**
   * For internal use only. Initializes the CAN peripherals.
   */
    void initPeripherals();
  
    /**
   * Representation of a physical CAN peripheral of the microcontroller.
   * This might be connected and thus mapped accordingly to a logical CAN bus (bus numbers of the vehicles).
   */
    class Peripheral {
        /**
       * The logical CAN bus that is connected to this interface.
       * Starts of a INVALID_BUS_INDEX to indicate no connection.
       */
        uint8_t mappedBus;
      public:
        /**
       * Defines the CAN peripheral this object is representing.
       */
        CAN_HandleTypeDef *const handle;
      
        /**
       * The first CAN filter bank that can be used with this peripheral.
       */
        const uint8_t firstFilterIndex;
      
        explicit Peripheral(CAN_HandleTypeDef *handle, uint8_t firstFilterIndex);
      
        void mapBus(uint8_t bus);
        [[nodiscard]] uint8_t getBus() const;
    };
}

#endif
