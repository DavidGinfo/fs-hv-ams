set(TARGET Module_FreeRTOS)

create_module_library(STATIC)

module_import(processor_families BaseConf PROCESSOR_FAMILIES)
module_import(mcu_families BaseConf MCU_FAMILIES)

file(GLOB_RECURSE MODULE_FREERTOS_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/*.*")
target_sources(${TARGET} PRIVATE ${MODULE_FREERTOS_SOURCES})
target_include_directories(${TARGET} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

target_include_directories(${TARGET} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/config/common")
target_sources(${TARGET} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/config/common/RuntimeStats.cpp")

if (Cortex-M4F IN_LIST processor_families)
    message(STATUS "Using the cortex_m4f FreeRTOS port")

    target_sources(${TARGET} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/ports/cortex_m4f/port.c")
    target_include_directories(${TARGET} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/ports/cortex_m4f")
elseif (Cortex-M7 IN_LIST processor_families)
    #message(STATUS "Using the cortex_m7 FreeRTOS port")

    #target_sources(${TARGET} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/ports/cortex_m7/port.c")
    #target_include_directories(${TARGET} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/ports/cortex_m7")
    message(STATUS "Using the cortex_m4f FreeRTOS port")

    target_sources(${TARGET} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/ports/cortex_m4f/port.c")
    target_include_directories(${TARGET} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/ports/cortex_m4f")
else ()
    message(FATAL_ERROR "No FreeRTOS port is available for this processor family")
endif ()

if (STM32 IN_LIST mcu_families)
    message(STATUS "Using the STM32 FreeRTOS configurations")

    target_include_directories(${TARGET} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/config/stm32")
else ()
    message(FATAL_ERROR "No FreeRTOS port is available for this microcontroller family")
endif ()

if (NOT DEFINED MODULE_FREERTOS_STATISTICS)
    set(MODULE_FREERTOS_STATISTICS OFF)
endif ()

target_compile_definitions(${TARGET} PRIVATE "RUNTIME_STATS=$<BOOL:${MODULE_FREERTOS_STATISTICS}>")

if (NOT DEFINED MODULE_FREERTOS_STATISTICS_SYSTEMVIEW_INTEGRATION)
    set(MODULE_FREERTOS_STATISTICS_SYSTEMVIEW_INTEGRATION OFF)
endif ()

target_compile_definitions(${TARGET} PUBLIC "MODULE_FREERTOS_STATISTICS_SYSTEMVIEW_INTEGRATION=$<BOOL:${MODULE_FREERTOS_STATISTICS_SYSTEMVIEW_INTEGRATION}>")

if (MODULE_FREERTOS_STATISTICS_SYSTEMVIEW_INTEGRATION)
    add_module_dependency(${TARGET} PUBLIC Segger_SystemView)
endif ()

add_module_dependency(${TARGET} PUBLIC ECU_Config)