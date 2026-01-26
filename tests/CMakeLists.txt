message(STATUS "> Found tests directory")

# Format
# label1,label2 (comma-separated)
set(DEFAULT_TESTS_LABELS "unit,tier_quick")

# Format:
# test_name | gtest_filter | labels (comma-separated!)
set(CTEST_ENTRIES
    "test_perft|Smoke/PerftSmokeTest.*|perft,tier_quick"
    "test_perft|Validation/PerftValidationTest.*|perft,tier_intermediate"
    "test_perft|Exhaustive/PerftExhaustiveTest.*|perft,tier_deep"
)

function(register_ctest_entries TEST_NAME)
    set(HAS_CUSTOM_ENTRIES FALSE)

    foreach(ENTRY IN LISTS CTEST_ENTRIES)
        string(REPLACE "|" ";" ENTRY_FIELDS "${ENTRY}")

        list(LENGTH ENTRY_FIELDS ENTRY_LEN)
        if(ENTRY_LEN LESS 3)
            message(FATAL_ERROR
                "Invalid CTEST_ENTRIES entry: '${ENTRY}' "
                "(expected: test|filter|labels with comma-seprated labels)"
            )
        endif()

        list(GET ENTRY_FIELDS 0 ENTRY_TEST)
        list(GET ENTRY_FIELDS 1 ENTRY_FILTER)
        list(GET ENTRY_FIELDS 2 ENTRY_LABELS)

        if(TEST_NAME STREQUAL ENTRY_TEST)
            set(HAS_CUSTOM_ENTRIES TRUE)

            set(CTEST_NAME "${TEST_NAME}::${ENTRY_FILTER}")

            add_test(
                NAME ${CTEST_NAME}
                COMMAND ${TEST_NAME} --gtest_filter=${ENTRY_FILTER}
            )

            string(REPLACE "," ";" ENTRY_LABELS_LIST "${ENTRY_LABELS}")
            set_tests_properties(${CTEST_NAME}
                PROPERTIES LABELS "${ENTRY_LABELS_LIST}"
            )
        endif()
    endforeach()

    if(NOT HAS_CUSTOM_ENTRIES)
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
        string(REPLACE "," ";" DEFAULT_TESTS_LABELS_LIST "${DEFAULT_TESTS_LABELS}")
        set_tests_properties(${TEST_NAME}
            PROPERTIES LABELS "${DEFAULT_TESTS_LABELS_LIST}"
        )
    endif()
endfunction()

file(GLOB_RECURSE TESTS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/bitbishop/test_*.cpp")
file(GLOB_RECURSE TESTS_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/bitbishop/*.hpp")

foreach(TEST_FILE ${TESTS_SOURCES})
    # Get file name (Without Extension)
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE} ${TESTS_HEADERS})
    target_link_libraries(${TEST_NAME} PRIVATE Bitbishop GTest::gtest GTest::gtest_main)
    target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    set_property(TARGET ${TEST_NAME} PROPERTY FOLDER Tests)
    register_ctest_entries(${TEST_NAME})
endforeach(TEST_FILE)
