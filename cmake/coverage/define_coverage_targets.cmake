# Brief:
# - Defines all the custom cmake targets used to run LLVM coverage from cmake:
#   * _coverage-run-tests
#.  * _coverage-merge      (depends on _coverage-run-tests)
#   * coverage-html         (depends on _coverage-merge)
#.  * coverage-summary      (depends on _coverage-merge)
#.  * coverage-json         (depends on _coverage-merge)
#.  * coverage-markdown     (depends on _coverage-merge)
#
# Docs:
# - https://clang.llvm.org/docs/SourceBasedCodeCoverage.html
# - https://llvm.org/docs/CommandGuide/llvm-cov.html

function(_find_preferred_llvm_tool OUT_PATH_VAR OUT_NAME_VAR TOOL_BASENAME)
    # Reads the active compiler's major version from CMAKE_CXX_COMPILER_VERSION.
    # Look in the compiler's own directory first to locate a matching llvm tool.
    # Fallback to the unversioned base tool name in the PATH if the binary was not found in compiler's directory.

    set(_search_hints "")

    if (CMAKE_CXX_COMPILER)
        get_filename_component(_compiler_bin_dir "${CMAKE_CXX_COMPILER}" DIRECTORY)
        list(APPEND _search_hints "${_compiler_bin_dir}")
    endif ()

    set(_candidate_names "${TOOL_BASENAME}")

    if (CMAKE_CXX_COMPILER_VERSION)
        string(REGEX MATCH "^[0-9]+" _compiler_major_version "${CMAKE_CXX_COMPILER_VERSION}")
        if (_compiler_major_version)
            list(INSERT _candidate_names 0 "${TOOL_BASENAME}-${_compiler_major_version}")
        endif ()
    endif ()

    # Build a "safe" dynamic cache filename for storing the tool's location in a CMake cache variable.
    string(REGEX REPLACE "[^A-Za-z0-9_]" "_" _tool_cache_suffix "${TOOL_BASENAME}")
    set(_tool_cache_var "_llvm_tool_${_tool_cache_suffix}")
    unset(${_tool_cache_var} CACHE)
    unset(${_tool_cache_var})

    find_program(${_tool_cache_var}
        NAMES ${_candidate_names}
        HINTS ${_search_hints}
    )

    set(_selected_tool "${${_tool_cache_var}}")
    if (_selected_tool)
        get_filename_component(_selected_tool_name "${_selected_tool}" NAME)
        set(${OUT_PATH_VAR} "${_selected_tool}" PARENT_SCOPE)
        set(${OUT_NAME_VAR} "${_selected_tool_name}" PARENT_SCOPE)
    else ()
        set(${OUT_PATH_VAR} "" PARENT_SCOPE)
        set(${OUT_NAME_VAR} "" PARENT_SCOPE)
    endif ()
endfunction()


if (ENABLE_COVERAGE)
    set(_CAN_ENABLE_COVERAGE TRUE)

    if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(WARNING "Coverage requested, but compiler is ${CMAKE_CXX_COMPILER_ID}. LLVM coverage requires Clang.")
        set(_CAN_ENABLE_COVERAGE FALSE)
    endif()

    if (_CAN_ENABLE_COVERAGE)
        if (CMAKE_CXX_COMPILER_VERSION)
            string(REGEX MATCH "^[0-9]+" LLVM_TOOLCHAIN_MAJOR_VERSION "${CMAKE_CXX_COMPILER_VERSION}")
        else ()
            set(LLVM_TOOLCHAIN_MAJOR_VERSION "")
        endif ()

        _find_preferred_llvm_tool(LLVM_PROFDATA LLVM_PROFDATA_NAME llvm-profdata)
        _find_preferred_llvm_tool(LLVM_COV LLVM_COV_NAME llvm-cov)

        if (NOT LLVM_PROFDATA)
            message(WARNING "Required tool 'llvm-profdata' not found in PATH.")
            set(_CAN_ENABLE_COVERAGE FALSE)
        else ()
            if (LLVM_TOOLCHAIN_MAJOR_VERSION AND LLVM_PROFDATA_NAME STREQUAL "llvm-profdata-${LLVM_TOOLCHAIN_MAJOR_VERSION}")
                message(STATUS "Using compiler-matched llvm-profdata tool: ${LLVM_PROFDATA_NAME} (${LLVM_PROFDATA})")
            else ()
                message(STATUS "Using fallback llvm-profdata tool: ${LLVM_PROFDATA_NAME} (${LLVM_PROFDATA})")
            endif ()
        endif()

        if (NOT LLVM_COV)
            message(WARNING "Required tool 'llvm-cov' not found in PATH.")
            set(_CAN_ENABLE_COVERAGE FALSE)
        else ()
            if (LLVM_TOOLCHAIN_MAJOR_VERSION AND LLVM_COV_NAME STREQUAL "llvm-cov-${LLVM_TOOLCHAIN_MAJOR_VERSION}")
                message(STATUS "Using compiler-matched llvm-cov tool: ${LLVM_COV_NAME} (${LLVM_COV})")
            else ()
                message(STATUS "Using fallback llvm-cov tool: ${LLVM_COV_NAME} (${LLVM_COV})")
            endif ()
        endif()
    endif()

    if (NOT _CAN_ENABLE_COVERAGE)
        # Force the option to OFF in the cache so subsequent runs/scripts know it failed
        set(ENABLE_COVERAGE OFF CACHE BOOL "Enable LLVM coverage tools" FORCE)
        message(STATUS "LLVM coverage setup failed. Coverage targets will not be created.")
    else()
        message(STATUS "Enabling LLVM coverage tools")
        message(STATUS "Coverage Build: Downgrading CX macros to runtime for tests instrumentation.")

        add_compile_definitions(COVERAGE_BUILD) # see include/bitbishop/config.hpp

        add_compile_options(
            -fprofile-instr-generate
            -fcoverage-mapping
        )
        add_link_options(
            -fprofile-instr-generate
        )

        # Coverage tier selection
        # Must match the tier used by the CTest preset
        set(CTEST_PRESET "quick-validation-clang-debug"
            CACHE STRING "Coverage tier (quick-validation-clang-debug, intermediate-validation-clang-debug, deep-validation-clang-debug, full-suite-clang-debug)")

        set(COVERAGE_BASE_DIR "${CMAKE_BINARY_DIR}/coverage")
        set(COVERAGE_DIR "${COVERAGE_BASE_DIR}/${CTEST_PRESET}")
        set(PROFDATA_FILE "${COVERAGE_DIR}/coverage.profdata")
        set(TESTS_BIN_DIR "${CMAKE_BINARY_DIR}/tests")

        # Internal target to run tests for the specified ctest preset
        # This target is then used as a dependency for subsequent custom targets
        add_custom_target(_coverage-run-tests
            COMMAND ${CMAKE_CTEST_COMMAND} --preset ${CTEST_PRESET} --output-on-failure
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMENT "Running tests for preset ${CTEST_PRESET}"
            USES_TERMINAL
        )

        # Internal target to merge raw coverage profiles
        # Triggers automatically the tests before running
        # Creates the coverage directory if non-existing (indempotent)
        add_custom_target(_coverage-merge
            COMMAND ${CMAKE_COMMAND} -E make_directory "${COVERAGE_DIR}"
            COMMAND ${CMAKE_COMMAND}
                "-DCOVERAGE_DIR=${COVERAGE_DIR}"
                "-DPROFDATA_FILE=${PROFDATA_FILE}"
                "-DLLVM_PROFDATA=${LLVM_PROFDATA}"
                -P "${CMAKE_SOURCE_DIR}/cmake/coverage/target_coverage_merge.cmake"
            BYPRODUCTS "${PROFDATA_FILE}"
            COMMENT "Merging LLVM coverage profiles for ${CTEST_PRESET}"
            VERBATIM
        )
        add_dependencies(_coverage-merge _coverage-run-tests)

        # Public target allowing to generate html (css, js) coverage report
        # Triggers automatically the tests and coverage merge before running
        add_custom_target(coverage-html
            COMMAND ${CMAKE_COMMAND}
                "-DCOVERAGE_DIR=${COVERAGE_DIR}"
                "-DPROFDATA_FILE=${PROFDATA_FILE}"
                "-DTESTS_BIN_DIR=${TESTS_BIN_DIR}"
                "-DCTEST_PRESET=${CTEST_PRESET}"
                "-DLLVM_COV=${LLVM_COV}"
                "-DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}"
                "-DREPORT_MODE=html"
                -P "${CMAKE_SOURCE_DIR}/cmake/coverage/target_coverage_report.cmake"
            COMMENT "Generating LLVM HTML coverage report (${CTEST_PRESET})"
            VERBATIM
        )
        add_dependencies(coverage-html _coverage-merge)

        # Public target allowing to generate stdout console coverage report
        # Triggers automatically the tests and coverage merge before running
        add_custom_target(coverage-summary
            COMMAND ${CMAKE_COMMAND}
                "-DCOVERAGE_DIR=${COVERAGE_DIR}"
                "-DPROFDATA_FILE=${PROFDATA_FILE}"
                "-DTESTS_BIN_DIR=${TESTS_BIN_DIR}"
                "-DCTEST_PRESET=${CTEST_PRESET}"
                "-DLLVM_COV=${LLVM_COV}"
                "-DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}"
                "-DREPORT_MODE=console"
                -P "${CMAKE_SOURCE_DIR}/cmake/coverage/target_coverage_report.cmake"
            COMMENT "Generating LLVM coverage summary (${CTEST_PRESET})"
            VERBATIM
        )
        add_dependencies(coverage-summary _coverage-merge)

        # Public target allowing to generate json coverage report
        # Triggers automatically the tests and coverage merge before running
        add_custom_target(coverage-json
            COMMAND ${CMAKE_COMMAND}
                "-DCOVERAGE_DIR=${COVERAGE_DIR}"
                "-DPROFDATA_FILE=${PROFDATA_FILE}"
                "-DTESTS_BIN_DIR=${TESTS_BIN_DIR}"
                "-DCTEST_PRESET=${CTEST_PRESET}"
                "-DLLVM_COV=${LLVM_COV}"
                "-DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}"
                "-DREPORT_MODE=json"
                -P "${CMAKE_SOURCE_DIR}/cmake/coverage/target_coverage_report.cmake"
            COMMENT "Exporting LLVM coverage as JSON (${CTEST_PRESET})"
            VERBATIM
        )
        add_dependencies(coverage-json _coverage-merge)

        # Public target allowing to generate markdown coverage report
        # Triggers automatically the tests and coverage merge before running
        add_custom_target(coverage-markdown
            COMMAND ${CMAKE_COMMAND}
                "-DCOVERAGE_DIR=${COVERAGE_DIR}"
                "-DPROFDATA_FILE=${PROFDATA_FILE}"
                "-DTESTS_BIN_DIR=${TESTS_BIN_DIR}"
                "-DCTEST_PRESET=${CTEST_PRESET}"
                "-DLLVM_COV=${LLVM_COV}"
                "-DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}"
                "-DREPORT_MODE=markdown"
                -P "${CMAKE_SOURCE_DIR}/cmake/coverage/target_coverage_report.cmake"
            COMMENT "Generating GitHub Actions coverage summary (${CTEST_PRESET})"
            VERBATIM
        )
        add_dependencies(coverage-markdown _coverage-merge)

        # Public target allowing to generate a shields.io coverage badge
        # Triggers automatically the tests and coverage merge before running
        add_custom_target(coverage-shieldsio
            COMMAND ${CMAKE_COMMAND}
                "-DCOVERAGE_DIR=${COVERAGE_DIR}"
                "-DPROFDATA_FILE=${PROFDATA_FILE}"
                "-DTESTS_BIN_DIR=${TESTS_BIN_DIR}"
                "-DCTEST_PRESET=${CTEST_PRESET}"
                "-DLLVM_COV=${LLVM_COV}"
                "-DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}"
                "-DREPORT_MODE=shieldsio"
                -P "${CMAKE_SOURCE_DIR}/cmake/coverage/target_coverage_report.cmake"
            COMMENT "Generating Shields.io coverage badge (${CTEST_PRESET})"
            VERBATIM
        )
        add_dependencies(coverage-shieldsio _coverage-merge)
    endif ()
endif ()
