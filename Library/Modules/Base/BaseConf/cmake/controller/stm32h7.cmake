if (${LINKER_SCRIPT_NAME} MATCHES "STM32H725")
    message(STATUS "Detected a STM32H725 controller")

    list(APPEND mcu_families "STM32H725xx")
    add_global_definitions(STM32H725xx)
    set_source_files_properties("${PROJECT_SOURCE_DIR}/Core/Src/stm32h7xx_it.c" PROPERTIES LANGUAGE CXX)

    if (${LINKER_SCRIPT_NAME} MATCHES "STM32H725ZGTX")
        set(mcu_model "STM32H725ZGTX")
    else ()
        message(WARNING "Using an unknown controller of the STM32H725-family")
    endif ()
elseif (${LINKER_SCRIPT_NAME} MATCHES "STM32H723")
message(STATUS "Detected a STM32H723 controller")

    list(APPEND mcu_families "STM32H723xx")
    add_global_definitions(STM32H723xx)
    set_source_files_properties("${PROJECT_SOURCE_DIR}/Core/Src/stm32h7xx_it.c" PROPERTIES LANGUAGE CXX)

    if (${LINKER_SCRIPT_NAME} MATCHES "STM32H723ZGTX")
        set(mcu_model "STM32H723ZGTX")
    else ()
        message(WARNING "Using an unknown controller of the STM32H723-family")
    endif ()
elseif (${LINKER_SCRIPT_NAME} MATCHES "STM32H753")
    message(STATUS "Detected a STM32H753 controller")

    list(APPEND mcu_families "STM32H753xx")
    add_global_definitions(STM32H753xx)
    set_source_files_properties("${PROJECT_SOURCE_DIR}/Core/Src/stm32h7xx_it.c" PROPERTIES LANGUAGE CXX)

    if (${LINKER_SCRIPT_NAME} MATCHES "STM32H753ZITX")
        set(mcu_model "STM32H753ZITX")
    else ()
        message(WARNING "Using an unknown controller of the STM32H753-family")
    endif ()
else ()
    message(WARNING "Using an unknown controller of the STM32H7-family")
endif ()

list(APPEND mcu_families "STM32H7")
add_global_definitions(MCU_FAMILY_STM32H7=1)

include("${cmake_dir}/isa/cortex_m7.cmake")
