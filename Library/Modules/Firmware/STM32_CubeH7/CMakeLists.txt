set(TARGET Module_STM32_CubeH7)

create_module_library(STATIC)

file(GLOB_RECURSE MODULE_STM32_CUBEH7_SOURCES "${PROJECT_SOURCE_DIR}/Drivers/*.*")

module_import(mcu_families BaseConf MCU_FAMILIES)
if (STM32H725xx IN_LIST mcu_families)
    list(APPEND MODULE_STM32_CUBEH7_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/startup_stm32h725xx.s")
elseif(STM32H723xx IN_LIST mcu_families)
    list(APPEND MODULE_STM32_CUBEH7_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/startup_stm32h723xx.s")
elseif(STM32H753xx IN_LIST mcu_families)
    list(APPEND MODULE_STM32_CUBEH7_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/startup_stm32h753xx.s")
else ()
    message(FATAL_ERROR "This microcontroller is unsupported by the STM32_CubeH7 module")
endif ()

target_sources(${TARGET} PRIVATE ${MODULE_STM32_CUBEH7_SOURCES})

target_compile_definitions(${TARGET} PUBLIC DEBUG USE_HAL_DRIVER)
target_include_directories(${TARGET} PUBLIC
        "${PROJECT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc"
        "${PROJECT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy"
        "${PROJECT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32H7xx/Include"
        "${PROJECT_SOURCE_DIR}/Drivers/CMSIS/Include"
)

add_module_dependency(${TARGET} INTERFACE ECU_Config)
