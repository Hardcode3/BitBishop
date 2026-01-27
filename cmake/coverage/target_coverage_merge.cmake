# Brief:
# - Merges `.profraw` llvm coverage files into a single `.profdata` coverage file
#
# Inputs:
#   COVERAGE_DIR: directory containing `*.profraw` files
#   PROFDATA_FILE: path to the final `*.prodata` file
#   LLVM_PROFDATA: path to the `llvm-profdata` executable
#
# Usage:
#
# Running in script mode, spawning a dedicated cmake process at build time:
# COMMAND ${CMAKE_COMMAND}
#     "-DCOVERAGE_DIR=${COVERAGE_DIR}"
#     "-DPROFDATA_FILE=${PROFDATA_FILE}"
#     "-DLLVM_PROFDATA=${LLVM_PROFDATA}"
#     -P "${CMAKE_SOURCE_DIR}/cmake/coverage/target_coverage_merge.cmake"

file(GLOB PROFRAW_FILES "${COVERAGE_DIR}/*.profraw")

list(LENGTH PROFRAW_FILES NUM_FILES)
if(NUM_FILES EQUAL 0)
    message(FATAL_ERROR "No .profraw files found in ${COVERAGE_DIR}. Did you run the tests with the correct preset?")
endif()

message(STATUS "Merging ${NUM_FILES} coverage profile(s)...")

execute_process(
    COMMAND ${LLVM_PROFDATA} merge --sparse ${PROFRAW_FILES} -o ${PROFDATA_FILE}
    RESULT_VARIABLE RES
    ERROR_VARIABLE ERR
)

if(NOT RES EQUAL 0)
    message(FATAL_ERROR "llvm-profdata merge failed:\n${ERR}")
endif()

message(STATUS "Coverage data merged successfully: ${PROFDATA_FILE}")
