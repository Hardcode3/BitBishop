# Inputs:
#   COVERAGE_DIR
#   PROFDATA_FILE
#   LLVM_COV
#   CTEST_PRESET  -> Name of the CTest preset used
#   REPORT_MODE (optional): show|report|json|github-summary

if(NOT DEFINED REPORT_MODE)
    set(REPORT_MODE "show")
endif()
if(NOT DEFINED CTEST_PRESET)
    message(FATAL_ERROR "CTEST_PRESET must be defined")
endif()

# Determine the source directory
if(NOT DEFINED PROJECT_SOURCE_DIR)
    get_filename_component(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
endif()

# Run ctest in JSON mode to get all test commands
execute_process(
    COMMAND ${CMAKE_CTEST_COMMAND} -N --show-only=json-v1 --preset ${CTEST_PRESET}
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    OUTPUT_VARIABLE CTEST_JSON
    RESULT_VARIABLE RES
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(NOT RES EQUAL 0)
    message(FATAL_ERROR "Failed to query CTest for preset ${CTEST_PRESET}")
endif()

# Parse JSON to extract test executables
string(JSON TESTS_COUNT ERROR_VARIABLE JSON_ERROR LENGTH "${CTEST_JSON}" "tests")
if(JSON_ERROR)
    message(FATAL_ERROR "Failed to parse CTest JSON: ${JSON_ERROR}")
endif()

set(COVERAGE_BINS "")
math(EXPR TESTS_LAST "${TESTS_COUNT} - 1")

foreach(IDX RANGE ${TESTS_LAST})
    string(JSON TEST_COMMAND ERROR_VARIABLE JSON_ERROR GET "${CTEST_JSON}" "tests" ${IDX} "command")
    if(NOT JSON_ERROR)
        string(JSON EXE_PATH ERROR_VARIABLE JSON_ERROR GET "${TEST_COMMAND}" 0)
        if(NOT JSON_ERROR AND EXISTS "${EXE_PATH}")
            list(APPEND COVERAGE_BINS "${EXE_PATH}")
        endif()
    endif()
endforeach()

list(REMOVE_DUPLICATES COVERAGE_BINS)

if(NOT COVERAGE_BINS)
    message(FATAL_ERROR "No test executables found for preset ${CTEST_PRESET}")
endif()

message(STATUS "Coverage binaries for preset ${CTEST_PRESET}:")
foreach(BIN IN LISTS COVERAGE_BINS)
    message(STATUS "  ${BIN}")
endforeach()

# Build llvm-cov command with all binaries
set(COV_COMMAND "${LLVM_COV}")
if(REPORT_MODE STREQUAL "show")
    list(APPEND COV_COMMAND show)
elseif(REPORT_MODE STREQUAL "report" OR REPORT_MODE STREQUAL "github-summary")
    list(APPEND COV_COMMAND report)
elseif(REPORT_MODE STREQUAL "json")
    list(APPEND COV_COMMAND export)
else()
    message(FATAL_ERROR "Unknown REPORT_MODE: ${REPORT_MODE}")
endif()

# Add first binary
list(GET COVERAGE_BINS 0 FIRST_BIN)
list(APPEND COV_COMMAND "${FIRST_BIN}")

# Add additional binaries with -object flag
list(LENGTH COVERAGE_BINS NUM_BINS)
if(NUM_BINS GREATER 1)
    list(REMOVE_AT COVERAGE_BINS 0)
    foreach(BIN IN LISTS COVERAGE_BINS)
        list(APPEND COV_COMMAND "-object=${BIN}")
    endforeach()
endif()

# Add common options
list(APPEND COV_COMMAND "-instr-profile=${PROFDATA_FILE}")

# Add source path remapping to ensure source files are found
list(APPEND COV_COMMAND "-path-equivalence=${PROJECT_SOURCE_DIR},${PROJECT_SOURCE_DIR}")

# Ignore file paths containing tests
list(APPEND COV_COMMAND "-ignore-filename-regex=tests/.*")

# Add mode-specific options
if(REPORT_MODE STREQUAL "show")
    list(APPEND COV_COMMAND
        "-format=html"
        "-output-dir=${COVERAGE_DIR}/html"
        "-show-line-counts-or-regions"
        "-show-instantiations=false"
    )
    # Only include project source files, not system headers
    list(APPEND COV_COMMAND "${PROJECT_SOURCE_DIR}/src" "${PROJECT_SOURCE_DIR}/include")

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

elseif(REPORT_MODE STREQUAL "report")
    execute_process(
        COMMAND ${COV_COMMAND}
        RESULT_VARIABLE RES
        ERROR_VARIABLE ERR
    )
    if(NOT RES EQUAL 0)
        message(FATAL_ERROR "llvm-cov report failed:\n${ERR}")
    endif()

elseif(REPORT_MODE STREQUAL "github-summary")
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
