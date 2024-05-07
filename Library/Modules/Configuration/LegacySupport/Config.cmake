macro(${_module_hook_pre_config})
    include_directories("${LIBRARY_DIR}/include")
    file(GLOB_RECURSE LEGACY_SOURCES "${LIBRARY_DIR}/src/common/*.*")
    file(GLOB_RECURSE COMMON_IO_SOURCES "${LIBRARY_DIR}/src/IO/common/*.*")
    list(APPEND LEGACY_SOURCES ${COMMON_IO_SOURCES})

    module_registry_current_directory(legacy_module_dir)

    include(${legacy_module_dir}/features/ADC.cmake)
    include(${legacy_module_dir}/features/APA102.cmake)
    include(${legacy_module_dir}/features/Diablo.cmake)
    include(${legacy_module_dir}/features/PressureSensor.cmake)
    include(${legacy_module_dir}/features/TemperatureSensor.cmake)

    include(${legacy_module_dir}/features/CAN.cmake)
    include(${legacy_module_dir}/features/I2C.cmake)
    include(${legacy_module_dir}/features/UART.cmake)
    include(${legacy_module_dir}/features/SPI.cmake)

    include(${legacy_module_dir}/features/EEPROM.cmake)

    include(${legacy_module_dir}/features/DMA.cmake)
    include(${legacy_module_dir}/features/Checksum.cmake)
    include(${legacy_module_dir}/features/Physics.cmake)
    include(${legacy_module_dir}/features/Text.cmake)
    include(${legacy_module_dir}/features/EFuse.cmake)
    target_sources(${PROJECT_NAME}.elf PRIVATE ${LEGACY_SOURCES})
endmacro()
