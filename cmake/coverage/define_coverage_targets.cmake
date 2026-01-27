if (ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "Enabling LLVM coverage tools")

    add_compile_options(
        -fprofile-instr-generate
        -fcoverage-mapping
    )
    add_link_options(
        -fprofile-instr-generate
    )

    find_program(LLVM_PROFDATA llvm-profdata REQUIRED)
    find_program(LLVM_COV llvm-cov REQUIRED)

    # Coverage tier selection
    # Must match the tier used by the CTest preset
    set(CTEST_PRESET "quick-validation-clang-debug"
        CACHE STRING "Coverage tier (quick-validation-clang-debug, intermediate-validation-clang-debug, deep-validation-clang-debug, full-suite-clang-debug)")

    set(COVERAGE_BASE_DIR "${CMAKE_BINARY_DIR}/coverage")
    set(COVERAGE_DIR "${COVERAGE_BASE_DIR}/${CTEST_PRESET}")
    set(PROFDATA_FILE "${COVERAGE_DIR}/coverage.profdata")
    set(TESTS_BIN_DIR "${CMAKE_BINARY_DIR}/tests")

    # Run tests for the selected preset
    add_custom_target(coverage-run-tests
        COMMAND ${CMAKE_CTEST_COMMAND} --preset ${CTEST_PRESET} --output-on-failure
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Running tests for preset ${CTEST_PRESET}"
        USES_TERMINAL
    )

    # Merge raw profiles
    add_custom_target(coverage-merge
        COMMAND ${CMAKE_COMMAND} -E make_directory "${COVERAGE_DIR}"
        COMMAND ${CMAKE_COMMAND}
            "-DCOVERAGE_DIR=${COVERAGE_DIR}"
            "-DPROFDATA_FILE=${PROFDATA_FILE}"
            "-DLLVM_PROFDATA=${LLVM_PROFDATA}"
            -P "${CMAKE_SOURCE_DIR}/cmake/coverage/merge_coverage.cmake"
        BYPRODUCTS "${PROFDATA_FILE}"
        COMMENT "Merging LLVM coverage profiles for ${CTEST_PRESET}"
        VERBATIM
    )
    add_dependencies(coverage-merge coverage-run-tests)

    # Generate coverage reports
    add_custom_target(coverage-html
        COMMAND ${CMAKE_COMMAND}
            "-DCOVERAGE_DIR=${COVERAGE_DIR}"
            "-DPROFDATA_FILE=${PROFDATA_FILE}"
            "-DTESTS_BIN_DIR=${TESTS_BIN_DIR}"
            "-DCTEST_PRESET=${CTEST_PRESET}"
            "-DLLVM_COV=${LLVM_COV}"
            "-DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}"
            -P "${CMAKE_SOURCE_DIR}/cmake/coverage/coverage_report.cmake"
        COMMENT "Generating LLVM HTML coverage report (${CTEST_PRESET})"
        VERBATIM
    )
    add_dependencies(coverage-html coverage-merge)

    add_custom_target(coverage-summary
        COMMAND ${CMAKE_COMMAND}
            "-DCOVERAGE_DIR=${COVERAGE_DIR}"
            "-DPROFDATA_FILE=${PROFDATA_FILE}"
            "-DTESTS_BIN_DIR=${TESTS_BIN_DIR}"
            "-DCTEST_PRESET=${CTEST_PRESET}"
            "-DLLVM_COV=${LLVM_COV}"
            "-DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}"
            "-DREPORT_MODE=report"
            -P "${CMAKE_SOURCE_DIR}/cmake/coverage/coverage_report.cmake"
        COMMENT "Generating LLVM coverage summary (${CTEST_PRESET})"
        VERBATIM
    )
    add_dependencies(coverage-summary coverage-merge)

    add_custom_target(coverage-json
        COMMAND ${CMAKE_COMMAND}
            "-DCOVERAGE_DIR=${COVERAGE_DIR}"
            "-DPROFDATA_FILE=${PROFDATA_FILE}"
            "-DTESTS_BIN_DIR=${TESTS_BIN_DIR}"
            "-DCTEST_PRESET=${CTEST_PRESET}"
            "-DLLVM_COV=${LLVM_COV}"
            "-DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}"
            "-DREPORT_MODE=json"
            -P "${CMAKE_SOURCE_DIR}/cmake/coverage/coverage_report.cmake"
        COMMENT "Exporting LLVM coverage as JSON (${CTEST_PRESET})"
        VERBATIM
    )
    add_dependencies(coverage-json coverage-merge)

    # Generate GitHub Actions summary
    add_custom_target(coverage-github-summary
        COMMAND ${CMAKE_COMMAND}
            "-DCOVERAGE_DIR=${COVERAGE_DIR}"
            "-DPROFDATA_FILE=${PROFDATA_FILE}"
            "-DTESTS_BIN_DIR=${TESTS_BIN_DIR}"
            "-DCTEST_PRESET=${CTEST_PRESET}"
            "-DLLVM_COV=${LLVM_COV}"
            "-DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}"
            "-DREPORT_MODE=github-summary"
            -P "${CMAKE_SOURCE_DIR}/cmake/coverage/coverage_report.cmake"
        COMMENT "Generating GitHub Actions coverage summary (${CTEST_PRESET})"
        VERBATIM
    )
    add_dependencies(coverage-github-summary coverage-merge)

endif()
