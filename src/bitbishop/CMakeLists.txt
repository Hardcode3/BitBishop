message(STATUS "> Found src/bitbishop directory")

# Option to build shared libraries, default is OFF for static libraries
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

# Define the library target name and source folder
set(target_name Bitbishop)
set(target_folder_name bitbishop)

# Search and list all .cpp files in the source folder
file(GLOB_RECURSE ROOT_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/${target_folder_name}/*.cpp")
# Search and list all .hpp files in the include folder
file(GLOB_RECURSE ROOT_HEADERS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/${target_folder_name}/*.hpp")

# Organize source files in the IDE's file tree
source_group(TREE "${PROJECT_SOURCE_DIR}/src/${target_folder_name}" PREFIX "Source Files" FILES ${ROOT_SOURCES})
# Organize header files in the IDE's file tree
source_group(TREE "${PROJECT_SOURCE_DIR}/include/${target_folder_name}" PREFIX "Header Files" FILES ${ROOT_HEADERS})

# Create a static library target (or shared if BUILD_SHARED_LIBS is ON)
add_library(${target_name} STATIC "")
# Set version and major version for the target
set_target_properties(${target_name} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

# Define version and source root macros for use in the library code
target_compile_definitions(${target_name} PRIVATE
    CHESS_ENGINE_VERSION="${CMAKE_PROJECT_VERSION}"
    CHESS_ENGINE_VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR}
    CHESS_ENGINE_VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR}
    CHESS_ENGINE_VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH}
    CHESS_ENGINE_ROOT_DIR="${PROJECT_SOURCE_DIR}"
)

# Add sources and headers to the target
target_sources(${target_name}
    PRIVATE
        ${ROOT_SOURCES}
        ${ROOT_HEADERS}
)

# Link dependencies to the library
target_link_libraries(${target_name} PRIVATE spdlog::spdlog fmt::fmt)

# Specify include directories for build and install interfaces separately
# - BUILD_INTERFACE is used while building the library from source
# - INSTALL_INTERFACE is used when the library is consumed after installation
target_include_directories(${target_name}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Define the folder in the IDE where the target will appear
set_property(TARGET ${target_name} PROPERTY FOLDER Bitbishop)

# Library packaging section

## Install the static library to the lib folder and headers to the include folder
install(TARGETS ${target_name}
    EXPORT ChessEngineTargets
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)

## Install headers found in the include directory
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/${target_folder_name}"
    DESTINATION include
)

## Export the target's configuration files for `find_package()`
install(EXPORT ChessEngineTargets
    FILE ChessEngineTargets.cmake
    NAMESPACE ChessEngine::
    DESTINATION lib/cmake/ChessEngine
)

## Generate version information for the package config files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ChessEngineConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

## Generate the main package config file
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/ChessEngineConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/ChessEngineConfig.cmake"
    INSTALL_DESTINATION lib/cmake/ChessEngine
)

## Install the generated config files for `find_package()`
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/ChessEngineConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/ChessEngineConfigVersion.cmake"
    DESTINATION lib/cmake/ChessEngine
)
