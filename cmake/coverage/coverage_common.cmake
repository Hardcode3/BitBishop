
function(_cov_execute_command COMMAND OUT_LINES)
    if(NOT COMMAND)
        message(FATAL_ERROR "No COMMAND passed to write_markdown_summary")
    endif()

    execute_process(
        COMMAND ${COMMAND}
        OUTPUT_VARIABLE OUTPUT
        ERROR_VARIABLE ERR
        RESULT_VARIABLE RES
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if(NOT RES EQUAL 0)
        message(FATAL_ERROR "llvm-cov report failed:\n${ERR}")
    endif()

    string(REPLACE "\n" ";" LINES "${OUTPUT}")
    set(${OUT_LINES} "${LINES}" PARENT_SCOPE)
endfunction()

function(_cov_find_total_line COVERAGE_LINES OUT_LINE)
    set(TOTAL_LINE "")
    foreach(LINE IN LISTS COVERAGE_LINES)
        if(LINE MATCHES "^TOTAL")
            set(TOTAL_LINE "${LINE}")
            break()
        endif()
    endforeach()

    set(${OUT_LINE} "${TOTAL_LINE}" PARENT_SCOPE)
endfunction()

function(_cov_parse_totals TOTAL_LINE LINE_REGEX
                           OUT_PARSED OUT_REGION OUT_FUNC OUT_LINE OUT_BRANCH)
    set(PARSED FALSE)

    if(TOTAL_LINE MATCHES "${LINE_REGEX}")
        set(PARSED TRUE)
        set(${OUT_REGION} "${CMAKE_MATCH_2}" PARENT_SCOPE)
        set(${OUT_FUNC}   "${CMAKE_MATCH_3}" PARENT_SCOPE)
        set(${OUT_LINE}   "${CMAKE_MATCH_4}" PARENT_SCOPE)
        set(${OUT_BRANCH} "${CMAKE_MATCH_5}" PARENT_SCOPE)
    endif()

    set(${OUT_PARSED} "${PARSED}" PARENT_SCOPE)
endfunction()

function(_cov_coverage_to_color REGION_COV OUT_NAMED_COLOR OUT_ICON OUT_TEXT)
    string(REGEX MATCH "[0-9]+" NUM "${REGION_COV}")

    if(NUM GREATER 90)
        set(NAMED_COLOR "brightgreen")
        set(ICON "🟢")
        set(TEXT "Good")
    elseif(NUM GREATER 70)
        set(NAMED_COLOR "yellow")
        set(ICON "🟡")
        set(TEXT "Fair")
    else()
        set(NAMED_COLOR "red")
        set(ICON "🔴")
        set(TEXT "Needs Improvement")
    endif()

    set(${OUT_NAMED_COLOR} "${NAMED_COLOR}" PARENT_SCOPE)
    set(${OUT_ICON} "${ICON}" PARENT_SCOPE)
    set(${OUT_TEXT} "${TEXT}" PARENT_SCOPE)
endfunction()
