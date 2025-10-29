cmake_minimum_required(VERSION 3.10)

project(
    ChessEngine
    VERSION 0.1.0
    LANGUAGES CXX
    DESCRIPTION "Basic C++ chess engine project."
    HOMEPAGE_URL "https://github.com/Hardcode3/CppChessEngine"
)
configure_file(${PROJECT_SOURCE_DIR}/cmake/ChessEngine.h.in ${PROJECT_BINARY_DIR}/ChessEngine.h)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Activate folders to sort targets (useful on Visual Studio for instance)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Optionally compile the main executable program
set(COMPILE_EXEC "Compile executable targets or discard them" ON)

# Optionally compile the test suite using GTest
set(COMPILE_TESTS "Compile tests or discard them" ON)

# Optionally enable packaging and install/export configuration
set(COMPILE_PACKAGE "Packages and exports library and binaries" ON)

include(cmake/clang_format.cmake)
include(cmake/clang_tidy.cmake)
include(cmake/compile_options.cmake)
include(cmake/setup_pthread.cmake)

find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_subdirectory(src)

# Add the main executable if COMPILE_EXEC is ON
if (COMPILE_EXEC)
    add_subdirectory(main)
endif ()

# Add test targets and enable CTest if COMPILE_TESTS is ON
if (COMPILE_TESTS)
    enable_testing()
    find_package(GTest CONFIG REQUIRED)
    add_subdirectory(tests)
endif ()

# Setup packaging configuration and enable CPack if COMPILE_PACKAGE is ON
if (COMPILE_PACKAGE)
    set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
    set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
    if (NOT DEFINED CPACK_GENERATOR)
        set(CPACK_GENERATOR "TGZ" CACHE STRING "Default packaging type (e.g., TGZ, DEB, RPM)" FORCE)
    endif ()
    include(CPack)
endif ()
