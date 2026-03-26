message(STATUS "> Found main directory")

add_executable(sandbox sandbox.cpp)
target_link_libraries(sandbox PRIVATE Bitbishop spdlog::spdlog)

add_executable(bitbishop bitbishop.cpp)
target_link_libraries(bitbishop PRIVATE Bitbishop spdlog::spdlog)

set_property(
    TARGET
        sandbox
        bitbishop
    PROPERTY FOLDER executables
)
