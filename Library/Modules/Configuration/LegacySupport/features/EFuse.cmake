if(NOT DEFINED FEATURE_EFUSE)
    set(FEATURE_EFUSE false)
endif()

if(${FEATURE_EFUSE})
    add_global_definitions(FEATURE_EFUSE=1)

    file(GLOB_RECURSE EFUSE_SOURCES "${LIBRARY_DIR}/src/IO/fuses/*.*")
    list(APPEND LEGACY_SOURCES ${EFUSE_SOURCES})
else()
    add_global_definitions(FEATURE_EFUSE=0)
endif()