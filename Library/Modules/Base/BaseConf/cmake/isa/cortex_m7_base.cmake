add_global_compile_options(-mcpu=cortex-m7 -mthumb)
add_global_link_options(-mcpu=cortex-m7 -mthumb)

list(APPEND processor_families "Cortex-M" "Cortex-M7")
add_global_definitions(ISA_ARM_THUMB=1 PROCESSOR_FAMILY_CORTEX_M=1 PROCESSOR_FAMILY_CORTEX_M7=1)
