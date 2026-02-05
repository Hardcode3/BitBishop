# -----------------------------------------------------------------------------------
# Defines a CMake custom target 'clang-tidy' to run clang-tidy on project's sources.
#
# Strategy: Parallel & Incremental
# Instead of running one massive command, we define a dependency graph where:
# 1. Each source file corresponds to a dummy output file (*.tidy).
# 2. These outputs are generated in parallel by the build system (Ninja/Make).
# 3. Successful runs 'touch' the dummy file, enabling incremental builds (only
#    changed files are re-analyzed).
#
# It only works if clang-tidy is found.
# -----------------------------------------------------------------------------------

find_program(CLANG_TIDY_BIN NAMES clang-tidy)

if(CLANG_TIDY_BIN)
    message(STATUS "clang-tidy found: ${CLANG_TIDY_BIN}")

    file(GLOB_RECURSE CLANG_TIDY_SOURCES
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_SOURCE_DIR}/include/*.hpp"
        "${CMAKE_SOURCE_DIR}/main/*.cpp"
    )

    set(CLANG_TIDY_EXTRA_ARGS "")
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
        list(APPEND CLANG_TIDY_EXTRA_ARGS "--extra-arg=-isysroot" "--extra-arg=${MACOS_SDK}")
    endif()

    set(TIDY_OUTPUT_FILES)
    foreach(SRC ${CLANG_TIDY_SOURCES})
        # Calculate a unique dummy output path for every source file
        file(RELATIVE_PATH REL_SRC ${CMAKE_SOURCE_DIR} ${SRC})
        set(TIDY_OUT "${CMAKE_BINARY_DIR}/linting/${REL_SRC}.tidy")

        get_filename_component(TIDY_DIR ${TIDY_OUT} DIRECTORY)
        file(MAKE_DIRECTORY ${TIDY_DIR})

        # Define an independent build rule for this specific source file.
        # This allows Ninja/Make to schedule these jobs in parallel across CPU cores.
        add_custom_command(
            OUTPUT ${TIDY_OUT}
            COMMAND ${CLANG_TIDY_BIN}
                    ${SRC}
                    -p=${CMAKE_BINARY_DIR}     # Use CMake's compile_commands.json
                    ${CLANG_TIDY_EXTRA_ARGS}
            # Touch the timestamp file on success to mark this file as "done"
            COMMAND ${CMAKE_COMMAND} -E touch ${TIDY_OUT}
            DEPENDS ${SRC}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Tidying ${REL_SRC}"
            VERBATIM
        )
        list(APPEND TIDY_OUTPUT_FILES ${TIDY_OUT})
    endforeach()

    # The master target depends on all the individual dummy files, triggering the parallel build
    add_custom_target(clang-tidy
        DEPENDS ${TIDY_OUTPUT_FILES}
        COMMENT "Running clang-tidy"
    )

else()
    message(STATUS "clang-tidy not found. Skipping clang-tidy target.")
endif()
