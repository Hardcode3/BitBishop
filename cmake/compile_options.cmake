
if (MSVC)
    # constexpr usage with fmt requires the use of /utf-8 compile option on MSVC
    # [build] vcpkg_installed\x64-windows\include\fmt\base.h(458,28):
    #   error C2338: static_assert failed:
    #       'Unicode support requires compiling with /utf-8'
    add_compile_options(/utf-8)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(
        "$<$<CONFIG:Debug>:-O0>"
        "$<$<CONFIG:Debug>:-g>"
        "$<$<CONFIG:Debug>:-fno-inline>"
        "$<$<CONFIG:Debug>:-fno-inline-functions>"
        "$<$<CONFIG:Debug>:-fno-omit-frame-pointer>"
    )

    if(APPLE)
        # macOS specific: force_load
        add_link_options(
            "$<$<CONFIG:Debug>:-Wl,-force_load,$<TARGET_FILE:Bitbishop>>"
        )
    elseif(UNIX AND NOT APPLE)
        # Linux specific: --whole-archive
        # Note: Linux uses --whole-archive / --no-whole-archive pairs
        add_link_options(
            "$<$<CONFIG:Debug>:-Wl,--whole-archive>"
            "$<$<CONFIG:Debug>:-Wl,$<TARGET_FILE:Bitbishop>>"
            "$<$<CONFIG:Debug>:-Wl,--no-whole-archive>"
        )
    endif()
endif()
