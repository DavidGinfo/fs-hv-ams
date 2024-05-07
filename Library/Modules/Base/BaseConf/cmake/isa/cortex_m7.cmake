include("${CMAKE_CURRENT_LIST_DIR}/cortex_m7_base.cmake")

# FPU configuration
add_global_definitions(ARM_MATH_CM7 ARM_MATH_MATRIX_CHECK ARM_MATH_ROUNDING)
add_global_compile_options(-mfloat-abi=hard -mfpu=fpv5-d16)
add_global_link_options(-mfloat-abi=hard -mfpu=fpv5-d16)

list(APPEND processor_families "Cortex-M7")
add_global_definitions(PROCESSOR_FAMILY_CORTEX_M7=1)