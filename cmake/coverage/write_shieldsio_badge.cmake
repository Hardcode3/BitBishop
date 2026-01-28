# Brief:
# - Defines the public function `write_shieldsio_coverage_badge` to write a code coverage badge to a json file for
#   use with shields.io, markdown and gh-pages
#
# Usage:
#
# include("${CMAKE_CURRENT_LIST_DIR}/write_shieldsio_badge.cmake")
# write_shieldsio_coverage_badge(
#     COVERAGE_DIR       "${COVERAGE_DIR}"
#     PRESET             "${CTEST_PRESET}"
#     PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}"
#     COMMAND            ${COV_COMMAND}
# )

include(${CMAKE_CURRENT_LIST_DIR}/coverage_common.cmake)

function(write_shieldsio_coverage_badge)
    set(options "")
    set(oneValueArgs COVERAGE_DIR PRESET PROJECT_SOURCE_DIR)
    set(multiValueArgs COMMAND)

    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT ARG_COMMAND)
        message(FATAL_ERROR "No COMMAND passed to write_shieldsio_coverage_badge")
    endif()

    _cov_execute_command("${ARG_COMMAND}" COVERAGE_LINES)
    _cov_find_total_line("${COVERAGE_LINES}" TOTAL_LINE)

    set(RE_SEP "[ \t]+")
    set(RE_NUM "[0-9]+")
    set(RE_PCT "([0-9.]+%|-)")
    set(LINE_REGEX
        "^(.+)${RE_SEP}${RE_NUM}${RE_SEP}${RE_NUM}${RE_SEP}${RE_PCT}"
        "${RE_SEP}${RE_NUM}${RE_SEP}${RE_NUM}${RE_SEP}${RE_PCT}"
        "${RE_SEP}${RE_NUM}${RE_SEP}${RE_NUM}${RE_SEP}${RE_PCT}"
        "${RE_SEP}${RE_NUM}${RE_SEP}${RE_NUM}${RE_SEP}${RE_PCT}$"
    )
    string(JOIN "" LINE_REGEX ${LINE_REGEX})

    _cov_parse_totals(
        "${TOTAL_LINE}" "${LINE_REGEX}"
        PARSED REGION_COV FUNC_COV LINE_COV BRANCH_COV
    )

    if(PARSED)
        _cov_coverage_to_color("${REGION_COV}" BADGE_NAMED_COLOR BADGE_ICON BADGE_TEXT)
        set(SHIELDSIO_BADGE_FILE "${ARG_COVERAGE_DIR}/shieldsio_coverage_badge.json")
        configure_file(
            ${CMAKE_CURRENT_LIST_DIR}/shieldsio_coverage_badge.json.in
            ${SHIELDSIO_BADGE_FILE}
            @ONLY
        )
    else()
        message(WARNING "Could not parse TOTAL line from coverage output.")
        set(REGION_COV "0%")
    endif()

    message(STATUS "Shields.io badge written to: ${SHIELDSIO_BADGE_FILE}")
endfunction()
