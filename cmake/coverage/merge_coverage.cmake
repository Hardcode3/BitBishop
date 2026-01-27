# Inputs:
#   COVERAGE_DIR
#   PROFDATA_FILE
#   LLVM_PROFDATA

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
