# -----------------------------------------------------------------------------------
# Defines a CMake custom target 'clang-tidy' to run clang-tidy on project's sources.
# It only works if clang-tidy is found.
# -----------------------------------------------------------------------------------

find_program(CLANG_TIDY_BIN NAMES clang-tidy)

if(CLANG_TIDY_BIN)
    message(STATUS "clang-tidy found: ${CLANG_TIDY_BIN}")

    message(STATUS "Cmake source dir: ${CMAKE_SOURCE_DIR}")

    file(GLOB_RECURSE CLANG_TIDY_SOURCES
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_SOURCE_DIR}/main/*.cpp"
        # Note: headers are analyzed by inclusion
    )

    if(APPLE)
        # Clang-Tidy requires extra arguments on macOS:
        # - It "reconstructs" the compilation process
        # - And depends on the flags provided by `compile_commands.json`
        # - Apple Clang needs an explicit mention of the standard lib and the sdk root
        # Linux does not need this because headers are in a standard location
        # Windows does not need this either
        # MacOS is stricter here
        execute_process(
            COMMAND xcrun --show-sdk-path
            OUTPUT_VARIABLE MACOS_SDK
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    endif()

    add_custom_target(clang-tidy COMMENT "Running clang-tidy")

    foreach(SRC ${CLANG_TIDY_SOURCES})
        add_custom_command(
            TARGET clang-tidy
            POST_BUILD
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMAND ${CLANG_TIDY_BIN}
                    ${SRC}
                    -p=${CMAKE_BINARY_DIR}     # Use CMake's compile_commands.json
                    $<$<PLATFORM_ID:Darwin>:--extra-arg=-isysroot>
                    $<$<PLATFORM_ID:Darwin>:--extra-arg=${MACOS_SDK}>
            VERBATIM
        )
    endforeach()
else()
    message(STATUS "clang-tidy not found. Skipping clang-tidy target.")
endif()
