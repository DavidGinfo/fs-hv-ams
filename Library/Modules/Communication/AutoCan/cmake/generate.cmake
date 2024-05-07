
file(GLOB AUTO_CAN_TEMPLATES "${CAN_DIRECTORY}/Generator/template/*.*")
file(MAKE_DIRECTORY ${AUTO_CAN_CACHE_DBC})
file(MAKE_DIRECTORY ${AUTO_CAN_CACHE_TEMPLATE})
file(MAKE_DIRECTORY ${AUTO_CAN_CACHE_INCLUDE})
file(MAKE_DIRECTORY ${AUTO_CAN_CACHE_SOURCE})
file(MAKE_DIRECTORY ${AUTO_CAN_INCLUDE})
file(MAKE_DIRECTORY ${AUTO_CAN_SOURCE})

if (NOT AUTO_CAN_INPUTS)
    message(FATAL_ERROR "There are no CAN input files")
endif ()

function(copy_if_different _inputs _to _result_var)
    set(${_result_var} OFF PARENT_SCOPE)

    foreach (_input ${_inputs})
        get_filename_component(_filename ${_input} NAME)
        set(_output "${_to}/${_filename}")

        if (EXISTS "${_output}")
            file(READ ${_input} _input_data)
            file(READ ${_output} _output_data)

            if (_input_data STREQUAL _output_data)
                break()
            endif ()
        endif ()

        file(COPY_FILE ${_input} ${_output})
        set(${_result_var} ON PARENT_SCOPE)
    endforeach ()
endfunction()

copy_if_different("${AUTO_CAN_INPUTS}" ${AUTO_CAN_CACHE_DBC} AUTO_CAN_CHANGED)
copy_if_different("${AUTO_CAN_TEMPLATES}" ${AUTO_CAN_CACHE_TEMPLATE} AUTO_CAN_TEMPLATES_CHANGED)
#if (NOT AUTO_CAN_CHANGED AND NOT AUTO_CAN_TEMPLATES_CHANGED)
#    message(STATUS "The CAN .dbc's and templates are unchanged, skipping the generator")
#    return()
#endif ()

message(STATUS "Running the AutoCan generator")
module_python_run(
        FILE "${CAN_DIRECTORY}/Generator/generate.py"
        ARGS -s ${AUTO_CAN_CACHE_SOURCE} -c ${AUTO_CAN_CACHE_INCLUDE} -i ${AUTO_CAN_INPUTS}
        RESULT_VARIABLE AUTO_CAN_RESULT
        OUTPUT_VARIABLE AUTO_CAN_ERROR
        AUTO_INSTALL
)
if (NOT ${AUTO_CAN_RESULT} EQUAL 0)
    file(REMOVE_RECURSE ${AUTO_CAN_CACHE_DBC})
    message(FATAL_ERROR "The AutoCan generator failed, console output:\r\n${AUTO_CAN_ERROR}")
endif ()

file(GLOB AUTO_CAN_GENERATED_HEADERS "${AUTO_CAN_CACHE_INCLUDE}/*.h")
file(GLOB AUTO_CAN_GENERATED_SOURCES "${AUTO_CAN_CACHE_SOURCE}/*.*")
copy_if_different("${AUTO_CAN_GENERATED_HEADERS}" ${AUTO_CAN_INCLUDE} AUTO_CAN_CHANGED)
copy_if_different("${AUTO_CAN_GENERATED_SOURCES}" ${AUTO_CAN_SOURCE} AUTO_CAN_CHANGED)
