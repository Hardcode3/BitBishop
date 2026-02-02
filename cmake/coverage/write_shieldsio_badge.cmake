# Brief:
# - Defines the public function `write_shieldsio_coverage_badges` to write a code coverage badge to a json file for
#   use with shields.io, markdown and gh-pages
#
# Usage:
#
# include("${CMAKE_CURRENT_LIST_DIR}/write_shieldsio_badge.cmake")
# write_shieldsio_coverage_badges(
#     COVERAGE_DIR       "${COVERAGE_DIR}"
#     PRESET             "${CTEST_PRESET}"
#     PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}"
#     COMMAND            ${COV_COMMAND}
# )

include(${CMAKE_CURRENT_LIST_DIR}/coverage_common.cmake)

function(write_unit_shieldsio_coverage_badge COV_VALUE BADGE_LABEL BADGE_FNAME)
    _cov_coverage_to_color("${COV_VALUE}" BADGE_NAMED_COLOR BADGE_ICON BADGE_TEXT)
    set(SHIELDSIO_BADGE_FILE "${ARG_COVERAGE_DIR}/${BADGE_FNAME}")
    configure_file(
        ${CMAKE_CURRENT_LIST_DIR}/shieldsio_coverage_badge.json.in
        ${SHIELDSIO_BADGE_FILE}
        @ONLY
    )
    message(STATUS "Shields.io ${BADGE_LABEL} badge written to: ${SHIELDSIO_BADGE_FILE}")
endfunction()

function(write_shieldsio_coverage_badges)
    set(options "")
    set(oneValueArgs COVERAGE_DIR PRESET PROJECT_SOURCE_DIR)
    set(multiValueArgs COMMAND)

    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT ARG_COMMAND)
        message(FATAL_ERROR "No COMMAND passed to write_shieldsio_coverage_badges")
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
    message(STATUS "Extracted region coverage   ${REGION_COV}")
    message(STATUS "Extracted function coverage ${FUNC_COV}")
    message(STATUS "Extracted line coverage     ${LINE_COV}")
    message(STATUS "Extracted branch coverage   ${BRANCH_COV}")

    if(PARSED)
        write_unit_shieldsio_coverage_badge("${REGION_COV}" "Region Coverage" "shieldsio_region_coverage_badge.json")
        write_unit_shieldsio_coverage_badge("${FUNC_COV}" "Function Coverage" "shieldsio_function_coverage_badge.json")
        write_unit_shieldsio_coverage_badge("${LINE_COV}" "Line Coverage" "shieldsio_line_coverage_badge.json")
        write_unit_shieldsio_coverage_badge("${BRANCH_COV}" "Branch Coverage" "shieldsio_branch_coverage_badge.json")
    else()
        message(WARNING "Could not parse TOTAL line from coverage output.")
        set(REGION_COV "0%")
    endif()
endfunction()
