if(NOT DEFINED FEATURE_EEPROM)
    set(FEATURE_EEPROM false)
endif()

if(${FEATURE_EEPROM})
    add_global_definitions(FEATURE_EEPROM=1)

    file(GLOB_RECURSE EEPROM_SOURCES "${LIBRARY_DIR}/src/IO/EEPROM/*.*")
    list(APPEND LEGACY_SOURCES ${EEPROM_SOURCES})
else()
    add_global_definitions(FEATURE_EEPROM=0)
endif()