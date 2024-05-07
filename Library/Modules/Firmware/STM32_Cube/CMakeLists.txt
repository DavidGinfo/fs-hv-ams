set(TARGET Module_STM32_Cube)

create_module_library(INTERFACE)
module_import(mcu_families BaseConf MCU_FAMILIES)

if (STM32F4 IN_LIST mcu_families)
    add_module_dependency(${TARGET} INTERFACE STM32_CubeF4)
elseif (STM32G4 IN_LIST mcu_families)
    add_module_dependency(${TARGET} INTERFACE STM32_CubeG4)
elseif (STM32H7 IN_LIST mcu_families)
    add_module_dependency(${TARGET} INTERFACE STM32_CubeH7)
else ()
    message(FATAL_ERROR "Could not find an appropriate STM32Cube firmware for this microcontroller")
endif ()
