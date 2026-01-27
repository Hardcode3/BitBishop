# Inputs:
#   COVERAGE_DIR            Path to the directory where `*.profraw` are defined
#   PROFDATA_FILE           Path to the `.profdata` file generated from the merge of `*.profraw` files
#   LLVM_COV                Path to the `llvm-cov` executable
#   CTEST_PRESET:           Name of the CTest preset used
#   REPORT_MODE (optional): show|report|json|markdown
#
# Usage:
#
# Running in script mode, spawning a dedicated cmake process at build time:
# COMMAND ${CMAKE_COMMAND}
#     "-DCOVERAGE_DIR=${COVERAGE_DIR}"
#     "-DPROFDATA_FILE=${PROFDATA_FILE}"
#     "-DLLVM_PROFDATA=${LLVM_PROFDATA}"
#     -P "${CMAKE_SOURCE_DIR}/cmake/coverage/target_coverage_merge.cmake"

macro(coverage_validate_inputs)
    if(NOT DEFINED REPORT_MODE)
        message(FATAL_ERROR
            "REPORT_MODE must be defined and one of: html, console, json, markdown"
        )
    endif()

    set(_valid_modes html console json markdown)
    list(FIND _valid_modes "${REPORT_MODE}" _idx)
    if(_idx EQUAL -1)
        message(FATAL_ERROR
            "Invalid REPORT_MODE: ${REPORT_MODE}\n"
            "Valid values are: html, console, json, markdown"
        )
    endif()

    if(NOT DEFINED CTEST_PRESET)
        message(FATAL_ERROR "CTEST_PRESET must be defined")
    endif()

    foreach(req COVERAGE_DIR PROFDATA_FILE LLVM_COV)
        if(NOT DEFINED ${req})
            message(FATAL_ERROR "${req} must be defined")
        endif()
    endforeach()
endmacro()

macro(coverage_resolve_project_source_dir)
    if(NOT DEFINED PROJECT_SOURCE_DIR)
        get_filename_component(
            PROJECT_SOURCE_DIR
            "${CMAKE_CURRENT_LIST_DIR}/.."
            ABSOLUTE
        )
    endif()
endmacro()

macro(coverage_collect_test_binaries)
    execute_process(
        COMMAND ${CMAKE_CTEST_COMMAND} -N --show-only=json-v1 --preset ${CTEST_PRESET}
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        OUTPUT_VARIABLE CTEST_JSON
        OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE RES
    )

    if(NOT RES EQUAL 0)
        message(FATAL_ERROR "Failed to query CTest for preset ${CTEST_PRESET}")
    endif()

    string(JSON TESTS_COUNT ERROR_VARIABLE JSON_ERROR LENGTH "${CTEST_JSON}" "tests")
    if(JSON_ERROR)
        message(FATAL_ERROR "Failed to parse CTest JSON: ${JSON_ERROR}")
    endif()

    set(COVERAGE_BINS "")
    math(EXPR LAST "${TESTS_COUNT} - 1")

    foreach(IDX RANGE ${LAST})
        string(JSON CMD ERROR_VARIABLE JSON_ERROR GET "${CTEST_JSON}" "tests" ${IDX} "command")
        if(NOT JSON_ERROR)
            string(JSON EXE ERROR_VARIABLE JSON_ERROR GET "${CMD}" 0)
            if(NOT JSON_ERROR AND EXISTS "${EXE}")
                list(APPEND COVERAGE_BINS "${EXE}")
            endif()
        endif()
    endforeach()

    list(REMOVE_DUPLICATES COVERAGE_BINS)

    if(NOT COVERAGE_BINS)
        message(FATAL_ERROR "No test executables found for preset ${CTEST_PRESET}")
    endif()
endmacro()

macro(build_llvm_cov_command)
    set(COV_COMMAND "${LLVM_COV}")

    if(REPORT_MODE STREQUAL "html")
        list(APPEND COV_COMMAND show)
    elseif(REPORT_MODE STREQUAL "console" OR REPORT_MODE STREQUAL "markdown")
        list(APPEND COV_COMMAND report)
    elseif(REPORT_MODE STREQUAL "json")
        list(APPEND COV_COMMAND export)
    endif()

    list(GET COVERAGE_BINS 0 FIRST_BIN)
    list(APPEND COV_COMMAND "${FIRST_BIN}")

    list(LENGTH COVERAGE_BINS NUM_BINS)
    if(NUM_BINS GREATER 1)
        list(REMOVE_AT COVERAGE_BINS 0)
        foreach(BIN IN LISTS COVERAGE_BINS)
            list(APPEND COV_COMMAND "-object=${BIN}")
        endforeach()
    endif()

    list(APPEND COV_COMMAND
        "-instr-profile=${PROFDATA_FILE}"
        "-path-equivalence=${PROJECT_SOURCE_DIR},${PROJECT_SOURCE_DIR}"
        "-ignore-filename-regex=tests/.*"
    )
endmacro()

macro(run_llvm_cov)
    if(REPORT_MODE STREQUAL "html")
        list(APPEND COV_COMMAND
            "-format=html"
            "-output-dir=${COVERAGE_DIR}/html"
            "-show-line-counts-or-regions"
            "-show-instantiations=false"
            "${PROJECT_SOURCE_DIR}/src"
            "${PROJECT_SOURCE_DIR}/include"
        )

        execute_process(
            COMMAND ${COV_COMMAND}
            RESULT_VARIABLE RES
            ERROR_VARIABLE ERR
        )

        if(RES EQUAL 0)
            message(STATUS "HTML coverage report: ${COVERAGE_DIR}/html/index.html")
        else()
            message(FATAL_ERROR "llvm-cov show failed:\n${ERR}")
        endif()

    elseif(REPORT_MODE STREQUAL "console")
        execute_process(
            COMMAND ${COV_COMMAND}
            RESULT_VARIABLE RES
            ERROR_VARIABLE ERR
        )

        if(NOT RES EQUAL 0)
            message(FATAL_ERROR "llvm-cov report failed:\n${ERR}")
        endif()

    elseif(REPORT_MODE STREQUAL "markdown")
        include("${CMAKE_CURRENT_LIST_DIR}/write_github_summary.cmake")

        write_github_summary(
            COVERAGE_DIR       "${COVERAGE_DIR}"
            PRESET             "${CTEST_PRESET}"
            PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}"
            COMMAND            ${COV_COMMAND}
        )

    elseif(REPORT_MODE STREQUAL "json")
        execute_process(
            COMMAND ${COV_COMMAND}
            OUTPUT_FILE "${COVERAGE_DIR}/coverage.json"
            RESULT_VARIABLE RES
            ERROR_VARIABLE ERR
        )

        if(RES EQUAL 0)
            message(STATUS "JSON coverage export: ${COVERAGE_DIR}/coverage.json")
        else()
            message(FATAL_ERROR "llvm-cov export failed:\n${ERR}")
        endif()
    endif()
endmacro()


coverage_validate_inputs()
coverage_resolve_project_source_dir()
coverage_collect_test_binaries()

list(LENGTH COVERAGE_BINS COVERAGE_BINS_SIZE)
message(STATUS "Collected ${COVERAGE_BINS_SIZE} binaries for coverage")

build_llvm_cov_command()
run_llvm_cov()
