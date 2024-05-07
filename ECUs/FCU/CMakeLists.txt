include(../../Library/library.cmake)
cmake_minimum_required(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION_})

# project settings
project(FCU C CXX ASM)
set(LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/STM32G473RETx_FLASH.ld)
set(FEATURE_CAN true)
set(FEATURE_ADS8698 true)
set(FEATURE_SPI true)
set(FEATURE_EFUSE true)
set(FEATURE_EEPROM true)
library_ecu_target()

include_directories(Core/Inc)
file(GLOB_RECURSE SOURCES "Core/*.*")
library_ecu_sources(${SOURCES})

set(MODULE_FREERTOS_STATISTICS_SYSTEMVIEW_INTEGRATION ON)

add_module_dependency(${PROJECT_NAME}.elf PRIVATE Os)
add_module_dependency(${PROJECT_NAME}.elf PRIVATE STM32_Cube)
add_module_dependency(${PROJECT_NAME}.elf PRIVATE AutoCan)

library_post_config()