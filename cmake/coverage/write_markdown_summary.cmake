# Brief:
# - Defines the public function `write_markdown_summary` to write code coverage to a markdown file
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

function(_cov_write_header SUMMARY_FILE PRESET)
    file(WRITE "${SUMMARY_FILE}" "## 📊 Coverage Report - ${PRESET}\n\n")
    string(TIMESTAMP TS "%Y-%m-%d %H:%M:%S UTC" UTC)
    file(APPEND "${SUMMARY_FILE}" "*Generated: ${TS}*\n\n")
endfunction()

function(_cov_write_key_metrics SUMMARY_FILE REGION FUNC LINE BRANCH)
    file(APPEND "${SUMMARY_FILE}" "### 📈 Key Metrics\n\n")
    file(APPEND "${SUMMARY_FILE}" "| Metric | Coverage |\n|--------|----------|\n")
    file(APPEND "${SUMMARY_FILE}" "| **Region Coverage** | ${REGION} |\n")
    file(APPEND "${SUMMARY_FILE}" "| Function Coverage | ${FUNC} |\n")
    file(APPEND "${SUMMARY_FILE}" "| Line Coverage | ${LINE} |\n")
    file(APPEND "${SUMMARY_FILE}" "| Branch Coverage | ${BRANCH} |\n")

    _cov_coverage_to_color("${REGION}" NAMED_COLOR ICON TEXT)
    file(APPEND "${SUMMARY_FILE}" "\n**Coverage Status**: ${ICON} ${TEXT}\n\n")
endfunction()

function(_cov_write_detailed_table SUMMARY_FILE COVERAGE_LINES LINE_REGEX PROJECT_SOURCE_DIR)
    file(APPEND "${SUMMARY_FILE}" "### 📋 Detailed Coverage by File\n\n")
    file(APPEND "${SUMMARY_FILE}" "| File | Region % | Function % | Line % | Branch % |\n")
    file(APPEND "${SUMMARY_FILE}" "|------|----------|------------|--------|----------|\n")

    set(IN_TABLE FALSE)
    foreach(LINE IN LISTS COVERAGE_LINES)
        if(LINE MATCHES "^-+$")
            set(IN_TABLE TRUE)
            continue()
        endif()

        if(NOT IN_TABLE OR LINE MATCHES "^TOTAL")
            continue()
        endif()

        if(LINE MATCHES "${LINE_REGEX}")
            set(FILENAME   "${CMAKE_MATCH_1}")
            set(REGION_PCT "${CMAKE_MATCH_2}")
            set(FUNC_PCT   "${CMAKE_MATCH_3}")
            set(LINE_PCT   "${CMAKE_MATCH_4}")
            set(BRANCH_PCT "${CMAKE_MATCH_5}")

            string(STRIP "${FILENAME}" FILENAME)
            string(REPLACE "[build] " "" FILENAME "${FILENAME}")
            string(REPLACE "${PROJECT_SOURCE_DIR}/" "" FILENAME "${FILENAME}")

            if(BRANCH_PCT STREQUAL "-")
                set(BRANCH_PCT "N/A")
            endif()

            # Highlight low/high percentages
            foreach(VAR REGION_PCT FUNC_PCT LINE_PCT)
                if(${VAR} MATCHES "([0-9]+)\\.")
                    set(NUM "${CMAKE_MATCH_1}")
                    if(NUM GREATER 90)
                        set(${VAR} "**${${VAR}}** ✓")
                    elseif(NUM LESS 70)
                        set(${VAR} "${${VAR}} ⚠️")
                    endif()
                endif()
            endforeach()

            if(NOT BRANCH_PCT STREQUAL "N/A" AND BRANCH_PCT MATCHES "([0-9]+)\\.")
                set(NUM "${CMAKE_MATCH_1}")
                if(NUM GREATER 90)
                    set(BRANCH_PCT "**${BRANCH_PCT}** ✓")
                elseif(NUM LESS 70)
                    set(BRANCH_PCT "${BRANCH_PCT} ⚠️")
                endif()
            endif()

            file(APPEND "${SUMMARY_FILE}"
                "| ${FILENAME} | ${REGION_PCT} | ${FUNC_PCT} | ${LINE_PCT} | ${BRANCH_PCT} |\n")
        endif()
    endforeach()
endfunction()

function(write_markdown_summary)
    set(options "")
    set(oneValueArgs COVERAGE_DIR PRESET PROJECT_SOURCE_DIR)
    set(multiValueArgs COMMAND)

    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT ARG_COMMAND)
        message(FATAL_ERROR "write_markdown_summary: No COMMAND passed")
    endif()

    set(SUMMARY_FILE "${ARG_COVERAGE_DIR}/github_summary.md")

    # Regex for parsing coverage
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

    _cov_execute_command("${ARG_COMMAND}" COVERAGE_LINES)
    _cov_find_total_line("${COVERAGE_LINES}" TOTAL_LINE)
    _cov_parse_totals(
        "${TOTAL_LINE}" "${LINE_REGEX}"
        PARSED REGION_COV FUNC_COV LINE_COV BRANCH_COV
    )

    if(PARSED)
        _cov_write_header("${SUMMARY_FILE}" "${ARG_PRESET}")
        _cov_write_key_metrics("${SUMMARY_FILE}" "${REGION_COV}" "${FUNC_COV}" "${LINE_COV}" "${BRANCH_COV}")
        _cov_write_detailed_table("${SUMMARY_FILE}" "${COVERAGE_LINES}" "${LINE_REGEX}" "${ARG_PROJECT_SOURCE_DIR}")
    else()
        message(WARNING "Could not parse TOTAL line from coverage output.")
    endif()

    message(STATUS "GitHub Markdown summary written to: ${SUMMARY_FILE}")
endfunction()
