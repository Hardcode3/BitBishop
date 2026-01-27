
macro(_cov_execute_command)
    if(NOT ARG_COMMAND)
        message(FATAL_ERROR "No command passed to write_github_summary")
    endif()

    execute_process(
        COMMAND ${ARG_COMMAND}
        OUTPUT_VARIABLE COVERAGE_OUTPUT
        ERROR_VARIABLE ERR
        RESULT_VARIABLE RES
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if(NOT RES EQUAL 0)
        message(FATAL_ERROR "llvm-cov report failed:\n${ERR}")
    endif()

    string(REPLACE "\n" ";" COVERAGE_LINES "${COVERAGE_OUTPUT}")
endmacro()

macro(_cov_find_total_line)
    set(TOTAL_LINE "")
    foreach(LINE IN LISTS COVERAGE_LINES)
        if(LINE MATCHES "^TOTAL")
            set(TOTAL_LINE "${LINE}")
            break()
        endif()
    endforeach()
endmacro()

macro(_cov_parse_totals)
    set(PARSED FALSE)
    if(TOTAL_LINE MATCHES "${LINE_REGEX}")
        set(REGION_COV "${CMAKE_MATCH_2}")
        set(FUNC_COV   "${CMAKE_MATCH_3}")
        set(LINE_COV   "${CMAKE_MATCH_4}")
        set(BRANCH_COV "${CMAKE_MATCH_5}")
        set(PARSED TRUE)
    endif()
endmacro()

macro(_cov_write_header)
    file(WRITE "${SUMMARY_FILE}" "## 📊 Coverage Report - ${ARG_PRESET}\n\n")
    string(TIMESTAMP TS "%Y-%m-%d %H:%M:%S UTC" UTC)
    file(APPEND "${SUMMARY_FILE}" "*Generated: ${TS}*\n\n")
endmacro()

macro(_cov_write_key_metrics)
    file(APPEND "${SUMMARY_FILE}" "### 📈 Key Metrics\n\n")
    file(APPEND "${SUMMARY_FILE}" "| Metric | Coverage |\n|--------|----------|\n")
    file(APPEND "${SUMMARY_FILE}" "| **Region Coverage** | ${REGION_COV} |\n")
    file(APPEND "${SUMMARY_FILE}" "| Function Coverage | ${FUNC_COV} |\n")
    file(APPEND "${SUMMARY_FILE}" "| Line Coverage | ${LINE_COV} |\n")
    file(APPEND "${SUMMARY_FILE}" "| Branch Coverage | ${BRANCH_COV} |\n")

    string(REGEX MATCH "[0-9]+" COV_NUM "${REGION_COV}")

    if(COV_NUM GREATER 90)
        set(BADGE_COLOR "🟢")
        set(BADGE_TEXT "Good")
    elseif(COV_NUM GREATER 70)
        set(BADGE_COLOR "🟡")
        set(BADGE_TEXT "Fair")
    else()
        set(BADGE_COLOR "🔴")
        set(BADGE_TEXT "Needs Improvement")
    endif()

    file(APPEND "${SUMMARY_FILE}" "\n**Coverage Status**: ${BADGE_COLOR} ${BADGE_TEXT}\n\n")
endmacro()

macro(_cov_write_detailed_table)
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
            string(REPLACE "${ARG_PROJECT_SOURCE_DIR}/" "" FILENAME "${FILENAME}")

            if(BRANCH_PCT STREQUAL "-")
                set(BRANCH_PCT "N/A")
            endif()

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
                "| ${FILENAME} | ${REGION_PCT} | ${FUNC_PCT} | ${LINE_PCT} | ${BRANCH_PCT} |\n"
            )
        endif()
    endforeach()
endmacro()

function(write_github_summary)
    set(options "")
    set(oneValueArgs COVERAGE_DIR PRESET PROJECT_SOURCE_DIR)
    set(multiValueArgs COMMAND)

    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(SUMMARY_FILE "${ARG_COVERAGE_DIR}/github_summary.md")

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

    _cov_execute_command()
    _cov_write_header()
    _cov_find_total_line()
    _cov_parse_totals()

    if(PARSED)
        _cov_write_key_metrics()
    else()
        message(WARNING "Could not parse TOTAL line from coverage output.")
    endif()

    _cov_write_detailed_table()

    message(STATUS "GitHub Markdown summary written to: ${SUMMARY_FILE}")
endfunction()
