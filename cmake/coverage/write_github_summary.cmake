function(write_github_summary)
    set(options "")
    set(oneValueArgs COVERAGE_DIR PRESET PROJECT_SOURCE_DIR)
    set(multiValueArgs COMMAND)

    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT ARG_COMMAND)
        message(FATAL_ERROR "No command passed to write_github_summary")
    endif()

    # Execute llvm-cov
    execute_process(
        COMMAND ${ARG_COMMAND}
        OUTPUT_VARIABLE COVERAGE_OUTPUT
        RESULT_VARIABLE RES
        ERROR_VARIABLE ERR
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if(NOT RES EQUAL 0)
        message(FATAL_ERROR "llvm-cov report failed:\n${ERR}")
    endif()

    set(SUMMARY_FILE "${ARG_COVERAGE_DIR}/github_summary.md")
    file(WRITE "${SUMMARY_FILE}" "## 📊 Coverage Report - ${ARG_PRESET}\n\n")
    string(TIMESTAMP TIMESTAMP "%Y-%m-%d %H:%M:%S UTC" UTC)
    file(APPEND "${SUMMARY_FILE}" "*Generated: ${TIMESTAMP}*\n\n")

    # Split output into lines
    string(REPLACE "\n" ";" COVERAGE_LINES "${COVERAGE_OUTPUT}")

    # -------------------------------------------------------------------------
    # REGEX SETUP
    # -------------------------------------------------------------------------
    # [ \t]+       matches one or more spaces/tabs
    # [0-9]+       matches integers
    # ([0-9.]+%|-) matches percentages like "100.00%" or "-" (for empty)

    set(RE_SEP "[ \t]+")
    set(RE_NUM "[0-9]+")
    set(RE_PCT "([0-9.]+%|-)")

    # The llvm-cov columns are:
    # 1. Filename (Group 1)
    # 2. Regions:    Total, Missed, Cover (Group 2)
    # 3. Functions:  Total, Missed, Cover (Group 3)
    # 4. Lines:      Total, Missed, Cover (Group 4)
    # 5. Branches:   Total, Missed, Cover (Group 5)

    # We construct the regex with explicit separators between EVERY column
    set(LINE_REGEX "^(.+)${RE_SEP}${RE_NUM}${RE_SEP}${RE_NUM}${RE_SEP}${RE_PCT}${RE_SEP}${RE_NUM}${RE_SEP}${RE_NUM}${RE_SEP}${RE_PCT}${RE_SEP}${RE_NUM}${RE_SEP}${RE_NUM}${RE_SEP}${RE_PCT}${RE_SEP}${RE_NUM}${RE_SEP}${RE_NUM}${RE_SEP}${RE_PCT}$")

    # -------------------------------------------------------------------------
    # 1. PARSE TOTALS
    # -------------------------------------------------------------------------
    set(TOTAL_LINE "")
    # Find the line starting with TOTAL
    foreach(LINE IN LISTS COVERAGE_LINES)
        if(LINE MATCHES "^TOTAL")
            set(TOTAL_LINE "${LINE}")
            break()
        endif()
    endforeach()

    if(TOTAL_LINE AND TOTAL_LINE MATCHES "${LINE_REGEX}")
        # Capture groups for percentages are at indices 2, 3, 4, 5
        set(REGION_COV "${CMAKE_MATCH_2}")
        set(FUNC_COV   "${CMAKE_MATCH_3}")
        set(LINE_COV   "${CMAKE_MATCH_4}")
        set(BRANCH_COV "${CMAKE_MATCH_5}")

        file(APPEND "${SUMMARY_FILE}" "### 📈 Key Metrics\n\n")
        file(APPEND "${SUMMARY_FILE}" "| Metric | Coverage |\n|--------|----------|\n")
        file(APPEND "${SUMMARY_FILE}" "| **Region Coverage** | ${REGION_COV} |\n")
        file(APPEND "${SUMMARY_FILE}" "| Function Coverage | ${FUNC_COV} |\n")
        file(APPEND "${SUMMARY_FILE}" "| Line Coverage | ${LINE_COV} |\n")
        file(APPEND "${SUMMARY_FILE}" "| Branch Coverage | ${BRANCH_COV} |\n")

        # Badge Logic
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
    else()
        message(WARNING "Could not parse TOTAL line from coverage output.")
    endif()

    # -------------------------------------------------------------------------
    # 2. DETAILED TABLE
    # -------------------------------------------------------------------------
    file(APPEND "${SUMMARY_FILE}" "### 📋 Detailed Coverage by File\n\n")
    file(APPEND "${SUMMARY_FILE}" "| File | Region % | Function % | Line % | Branch % |\n")
    file(APPEND "${SUMMARY_FILE}" "|------|----------|------------|--------|----------|\n")

    set(IN_TABLE FALSE)
    foreach(LINE IN LISTS COVERAGE_LINES)
        # 1. Detect Header Separator (e.g. "------------------")
        if(LINE MATCHES "^-+$")
            set(IN_TABLE TRUE)
            continue()
        endif()

        # 2. Skip lines until we find the separator, and skip the TOTAL line here
        if(NOT IN_TABLE OR LINE MATCHES "^TOTAL")
            continue()
        endif()

        # 3. Match the Data Lines
        if(LINE MATCHES "${LINE_REGEX}")
            set(FILENAME   "${CMAKE_MATCH_1}")
            set(REGION_PCT "${CMAKE_MATCH_2}")
            set(FUNC_PCT   "${CMAKE_MATCH_3}")
            set(LINE_PCT   "${CMAKE_MATCH_4}")
            set(BRANCH_PCT "${CMAKE_MATCH_5}")

            # Clean up filename (greedy match might eat trailing spaces)
            string(STRIP "${FILENAME}" FILENAME)

            # Remove [build] prefix if present (common in some CI logs)
            string(REPLACE "[build] " "" FILENAME "${FILENAME}")

            # Make path relative
            string(REPLACE "${ARG_PROJECT_SOURCE_DIR}/" "" FILENAME "${FILENAME}")

            if(BRANCH_PCT STREQUAL "-")
                set(BRANCH_PCT "N/A")
            endif()

            # Formatting: Add Bold/Warning icons
            foreach(PCT_VAR REGION_PCT FUNC_PCT LINE_PCT)
                if(${PCT_VAR} MATCHES "([0-9]+)\\.")
                    set(NUM "${CMAKE_MATCH_1}")
                    if(NUM GREATER 90)
                        set(${PCT_VAR} "**${${PCT_VAR}}** ✓")
                    elseif(NUM LESS 70)
                        set(${PCT_VAR} "${${PCT_VAR}} ⚠️")
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

            file(APPEND "${SUMMARY_FILE}" "| ${FILENAME} | ${REGION_PCT} | ${FUNC_PCT} | ${LINE_PCT} | ${BRANCH_PCT} |\n")
        endif()
    endforeach()

    message(STATUS "GitHub Markdown summary written to: ${SUMMARY_FILE}")

endfunction()
