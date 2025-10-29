
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  # Required for pthread libs to be detected properly by cmake while configuring
  # It's a requirement for spdlog external library
  # Error log:
  # 'Could NOT find Threads (missing: Threads_FOUND)'
  #  build/clang_debug/vcpkg_installed/x64-linux/share/spdlog/spdlogConfig.cmake:30 (find_package)
  # Tried installing libs and checking availability with the PATH: everything was ok but cmake still couldn't find it
  # Tried libs: libpthread-stubs0-dev / libatomic1 / gcc-multilib / g++-multilib / libc6-dev / libc++-dev / libc++abi-dev
  # Finally found a workaround by forcing the pthread flags
  set(CMAKE_THREAD_LIBS_INIT "-pthread" CACHE STRING "")
  set(CMAKE_HAVE_THREADS_LIBRARY 1 CACHE BOOL "")
  set(CMAKE_USE_PTHREADS_INIT 1 CACHE BOOL "")
endif()
