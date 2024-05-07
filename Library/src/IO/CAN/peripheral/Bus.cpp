#include <IO/CAN/Bus.h>
#include <IO/CAN/Peripheral.h>
#include <IO/CAN/Config.h>

#if FD_CAN

namespace CAN {
  const uint32_t FILTER_EMPTY_VALUE = 0x7FF;
  
  uint32_t toFilterId(uint32_t actualId) {
      return actualId << 5U;
  }
  
  Bus bus1(0, BUS_1_PERIPHERAL);
  Bus bus2(1, BUS_2_PERIPHERAL);
  Bus bus3(2, BUS_3_PERIPHERAL);
  Bus *buses[BUS_COUNT] = {&bus1, &bus2, &bus3};
  
  void initBuses() {
      bus1.init();
      bus2.init();
      bus3.init();
  }
}

using namespace CAN;


Bus::Bus(uint8_t index, CAN::Peripheral *peripheral) :
        peripheral(peripheral),
        currentFilter(),
        index(index) {}

        
void Bus::init() {
    if (isValid()) {
        handle = peripheral->handle;
        currentFilterId = peripheral->firstFilterIndex;
        peripheral->mapBus(index);
    }
}

void Bus::setManager(Manager *newManager) {
    if (!isValid()) {
        Debug::exception();
    }
    
    if (manager != nullptr) {
        Debug::exception();
    }
    
    manager = newManager;
}

Manager *Bus::getManager() {
    return manager;
}

void Bus::addMessageToFilters(uint32_t id, bool extended) {
    if (!isValid()) {
        Debug::exception();
    }
    
    if (currentFilterMessageCount == 2) {
        if (currentFilterId - peripheral->firstFilterIndex >= CAN::FILTER_COUNT) {
            Debug::exception();
        }
        
        currentFilterId++;
        currentFilterMessageCount = 0;
    }
    
    switch (currentFilterMessageCount) {
        case 0:
            currentFilter.IdType = extended ? FDCAN_EXTENDED_ID : FDCAN_STANDARD_ID;
            currentFilter.FilterIndex = currentFilterId;
            currentFilter.FilterType = FDCAN_FILTER_DUAL;
            currentFilter.FilterConfig = FILTER_FIFO;
            currentFilter.FilterID2 = toFilterId(id);
            currentFilter.FilterID1 = FILTER_EMPTY_VALUE;
            currentFilterMessageCount++;
            break;
        
        case 1:
            currentFilter.FilterID1 = toFilterId(id);
            currentFilterMessageCount++;
            break;
        
        default:
            Debug::exception();
    }
    
    if (HAL_FDCAN_ConfigFilter(handle, &currentFilter) != HAL_OK) {
        Debug::exception();
    }
}

bool Bus::isValid() {
    return peripheral != nullptr;
}

#endif

#if !FD_CAN

namespace CAN {
  const uint32_t FILTER_EMPTY_VALUE = 0xFFFF;
  
  uint32_t toFilterId(uint32_t actualId) {
      return actualId << 5U;
  }
  
  Bus bus1(0, BUS_1_PERIPHERAL);
  Bus bus2(1, BUS_2_PERIPHERAL);
  Bus bus3(2, BUS_3_PERIPHERAL);
  Bus *buses[BUS_COUNT] = {&bus1, &bus2, &bus3};
  
  void initBuses() {
      bus1.init();
      bus2.init();
      bus3.init();
  }
}

using namespace CAN;


Bus::Bus(uint8_t index, CAN::Peripheral *peripheral) :
        peripheral(peripheral),
        currentFilter(),
        index(index) {}

        
void Bus::init() {
    if (isValid()) {
        handle = peripheral->handle;
        currentFilterId = peripheral->firstFilterIndex;
        peripheral->mapBus(index);
    }
}

void Bus::setManager(Manager *newManager) {
    if (!isValid()) {
        Debug::exception();
    }
    
    if (manager != nullptr) {
        Debug::exception();
    }
    
    manager = newManager;
}

Manager *Bus::getManager() {
    return manager;
}

void Bus::addMessageToFilters(uint16_t id) {
    if (!isValid()) {
        Debug::exception();
    }
    
    if (currentFilterMessageCount == 4) {
        if (currentFilterId - peripheral->firstFilterIndex >= CAN::FILTER_COUNT) {
            Debug::exception();
        }
        
        currentFilterId++;
        currentFilterMessageCount = 0;
    }
    
    switch (currentFilterMessageCount) {
        case 0:
            currentFilter.FilterBank = currentFilterId;
            currentFilter.FilterActivation = CAN_FILTER_ENABLE;
            currentFilter.FilterFIFOAssignment = FILTER_FIFO;
            currentFilter.FilterMode = CAN_FILTERMODE_IDLIST;
            currentFilter.FilterScale = CAN_FILTERSCALE_16BIT;
            currentFilter.SlaveStartFilterBank = CAN::FIRST_SLAVE_FILTER_BANK;
            currentFilter.FilterIdLow = toFilterId(id);
            currentFilter.FilterIdHigh = FILTER_EMPTY_VALUE;
            currentFilter.FilterMaskIdLow = FILTER_EMPTY_VALUE;
            currentFilter.FilterMaskIdHigh = FILTER_EMPTY_VALUE;
            currentFilterMessageCount++;
            break;
        
        case 1:
            currentFilter.FilterIdHigh = toFilterId(id);
            currentFilterMessageCount++;
            break;
        
        case 2:
            currentFilter.FilterMaskIdLow = toFilterId(id);
            currentFilterMessageCount++;
            break;
        
        case 3:
            currentFilter.FilterMaskIdHigh = toFilterId(id);
            currentFilterMessageCount++;
            break;
        
        default:
            Debug::exception();
    }
    
    if (HAL_CAN_ConfigFilter(handle, &currentFilter) != HAL_OK) {
        Debug::exception();
    }
}

bool Bus::isValid() {
    return peripheral != nullptr;
}

#endif