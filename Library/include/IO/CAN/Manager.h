#pragma once

#include <util/Common.h>
#include <IO/CAN/Bus.h>
#include <IO/CAN/Message.h>
#include <data/collection/FixedCapacityArrayList.h>
#include <parallel/Queue.hpp>
#include <parallel/Task.hpp>
#include <parallel/notification/CountingNotification.h>


namespace CAN {
  inline constexpr uint8_t MAX_MESSAGE_SIZE = 8; // 8 bytes for regular CAN
  
  inline constexpr uint8_t MAX_RECEIVE_MESSAGES = 5; // Costs 4 bytes per message
  inline constexpr uint8_t RX_QUEUE_LENGTH = 8; // Costs sizeof(BufferContainer) per message
  
  inline constexpr uint8_t MAX_SEND_MESSAGES = 10; // Costs 4 bytes per message
  inline constexpr uint8_t TX_QUEUE_LENGTH = MAX_SEND_MESSAGES / 2; // Costs sizeof(BufferContainer) per message
  inline constexpr uint8_t TX_BUFFER_COUNT = 3; // The amount of transmit mailboxes, should always be 3 for STM32 bxCAN
  inline constexpr uint8_t TX_BUFFER_WAIT_TIME = 1; // Amount of time to wait for the notification before polling again
  

  // Container for the TX Buffer
  struct BufferContainer {
    #if FD_CAN
      FDCAN_RxHeaderTypeDef header;
    #else
        CAN_RxHeaderTypeDef header;
    #endif
      uint8_t data[MAX_MESSAGE_SIZE];
  };
  
  
  /**
   * This class handles communication with the CAN bus and offers a high level API. You must enable the, TX, RX0 and SCE
   * interrupts for the managed CAN controller, a correct priority is needed (see docs about interrupts).
   *
   * Usage:
   * - This is almost fully by default, read the generic CAN documentation for this
   * - Non-RTOS: You need to call sendMessages() periodically, the exact timing is irrelevant
   *
   * Manual usage:
   * - Register messages via addRx/TxMessage()
   * - Call init() to activate the bus, you cannot add messages later on
   * - RTOS: This class uses it's own tasks and timers, you don't need to do anything
   * - Non-RTOS: You need to call sendMessages() as described above, receiving messages is automatic as well
   *
   * RX performance tuning (RTOS only):
   * - Use a high interrupt priority if needed
   * - TODO Make the RX task priority adjustable
   * - TODO Add support for callbacks/notifications to the associated logic
   *
   * Implementation details:
   * - CAN peripherals and filters abstracted away
   * - RTOS TX: RTOS-Timer -> TX-Task -> HAL
   * - RTOS RX: Interrupt -> RTOS-Queue -> RX-Task
   * - Non-RTOS TX: sendMessages() -> HAL
   * - Non-RTOS RX: All logic inside the interrupt
   *
   * @author David Schwärzle <mailto:david.schwaerzle@highspeed-karlsruhe.de>
   */
  class Manager {
    public:
      // Lifetime control
      Manager(Manager &) = delete;
      
      Manager(Manager &&) = delete;
      
      /**
       * Initializes the filters and starts the CAN peripheral. Run this after registering the messages.
       */
      void init();
      
      /**
       * Adds a message with automatic handling to this instance. Whenever the message gets received this manager will update
       * the value in memory with a short delay.
       *
       * @param message the message that should be received
       */
      [[maybe_unused]] void addRxMessage(MessageBase &message);
      
      /**
       * Adds a TX message to this instance. This instance will automatically send the message over CAN according to its
       * specified cycle time.
       *
       * @param message the message that should be sent periodically
       * @param pushFunction the push function for this message.
       */
      [[maybe_unused]] void addTxMessage(MessageBase &message);
      
      /**
       * Handles interrupts for received messages. DO NOT CALL MANUALLY.
       */
      void handleRx();
      
      /**
       * Handles interrupts for completed transmissions. DO NOT CALL MANUALLY.
       */
      void handleTxComplete();
      
      /**
       * @param can the CAN bus that this manager uses
       * @param rxTaskName the name for the RX task that this class uses
       * @param txTaskName the name for the TX task that this class uses
       */
      Manager(Bus &bus, const char *rxTaskName, const char *txTaskName)
              : bus(bus), rxQueue(rxTaskName), txQueue(txTaskName), txBufferCounter(txTask),
                rxTask(Functional::Delegate<void()>::bind<&Manager::runRxTask>(*this), rxTaskName, RTOS::PriorityNormal),
                txTask(Functional::Delegate<void()>::bind<&Manager::runTxTask>(*this), txTaskName, RTOS::PriorityNormal) {}
      
      /**
       * Runs the rx task that saves messages with automatic handling in memory. DO NOT CALL MANUALLY.
       */
      [[noreturn]] void runRxTask();
      
      /**
       * Runs the tx task that sends messages in the queue. DO NOT CALL MANUALLY.
       */
      [[noreturn]] void runTxTask();
      
      /**
       * Enqueues all messages with a specific period for transmission. You should not use this manually.
       *
       * @param period the period of the messages
       */
      void enqueueTxMessages(uint16_t period);

      /**
       * Manually invoke the transmission of a CAN message.
       *
       * @param message the CAN message to send
       */
      void transmitManual(CAN::MessageBase &message);

      void transmitManualFromISR(CAN::MessageBase &message);

    
    private:
#if FD_CAN
      FDCAN_HandleTypeDef *can = nullptr;
#else
      CAN_HandleTypeDef *can = nullptr;
#endif
      Bus &bus;
      
      FixedCapacityArrayList<MessageBase*, MAX_RECEIVE_MESSAGES> rxMessages;
      FixedCapacityArrayList<MessageBase*, MAX_SEND_MESSAGES> txMessages;
      

      Parallel::Queue<CAN::BufferContainer, RX_QUEUE_LENGTH> rxQueue;
      Parallel::NormalTask rxTask;
      
      Parallel::Queue<MessageBase *, TX_QUEUE_LENGTH> txQueue;
      Parallel::NormalTask txTask;
      
      /**
       * Monitors the amount of free buffers.
       */
      Parallel::CountingNotification txBufferCounter;
  };
}