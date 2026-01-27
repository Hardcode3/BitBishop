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

if (ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "Enabling LLVM coverage tools")

    find_program(LLVM_PROFDATA llvm-profdata REQUIRED)
    find_program(LLVM_COV llvm-cov REQUIRED)

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

endif()
