if(NOT DEFINED FEATURE_CHECKSUM)
    set(FEATURE_CHECKSUM false)
endif()

if(${FEATURE_CHECKSUM})
    add_compile_options(-DFEATURE_CHECKSUM=1)

    list(APPEND LEGACY_SOURCES "${LIBRARY_DIR}/src/data/checksum/SimpleChecksum.cpp")
else()
    add_compile_options(-DFEATURE_CHECKSUM=0)
endif()

if(NOT DEFINED FEATURE_CRC8_SMALL)
    set(FEATURE_CAN false)
endif()

if(${FEATURE_CRC8_SMALL})
    add_compile_options(-DFEATURE_CRC8_SMALL=1)

    list(APPEND LEGACY_SOURCES "${LIBRARY_DIR}/src/data/checksum/CRC8_Small.cpp")
else()
    add_compile_options(-DFEATURE_CRC8_SMALL=0)
endif()

if(NOT DEFINED FEATURE_CRC16_SMALL)
    set(FEATURE_CAN false)
endif()

if(${FEATURE_CRC16_SMALL})
    add_compile_options(-DFEATURE_CRC16_SMALL=1)

    list(APPEND LEGACY_SOURCES "${LIBRARY_DIR}/src/data/checksum/CRC16_Small.cpp")
else()
    add_compile_options(-DFEATURE_CRC16_SMALL=0)
endif()

if(NOT DEFINED FEATURE_CRC8_FAST)
    set(FEATURE_CRC8_FAST false)
endif()

if(${FEATURE_CRC8_FAST})
    add_compile_options(-DFEATURE_CRC8_FAST=1)

    list(APPEND LEGACY_SOURCES "${LIBRARY_DIR}/src/data/checksum/CRC8_Fast.cpp")
else()
    add_compile_options(-DFEATURE_CRC8_FAST=0)
endif()
