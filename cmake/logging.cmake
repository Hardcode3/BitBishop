find_package(spdlog CONFIG REQUIRED)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Logging level set to INFO for Release builds.")
    add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO)
else()
    message(STATUS "Logging level set to DEBUG for Debug builds.")
    add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG)
endif()
