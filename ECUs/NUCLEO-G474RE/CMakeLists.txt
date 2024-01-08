include(../../Library/library.cmake)
cmake_minimum_required(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION_})

# project settings
project(NUCLEO-G474RE C CXX ASM)
set(CMAKE_CXX_STANDARD 20)
set(LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/STM32G474RETX_FLASH.ld)
set(FEATURE_CAN false)
set(FEATURE_UART true)
set(FEATURE_CRC16_SMALL true)
library_ecu_target()

include_directories(Core/Inc)
file(GLOB_RECURSE SOURCES "Core/*.*")
library_ecu_sources(${SOURCES})

set(MODULE_FREERTOS_STATISTICS_SYSTEMVIEW_INTEGRATION ON)

add_module_dependency(${PROJECT_NAME}.elf PRIVATE Os)
add_module_dependency(${PROJECT_NAME}.elf PRIVATE STM32_Cube)
add_module_dependency(${PROJECT_NAME}.elf PRIVATE AutoCan)

library_post_config()