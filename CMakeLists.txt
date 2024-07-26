cmake_minimum_required(VERSION 3.14)
project(trafficMonitor)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/305e5a238b3c8d11266fbafd85520fb6b3184851.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enableing test
enable_testing()

# Copy the dependent files of the project (cfg, ini files or files users feel like opening)
configure_file(${CMAKE_SOURCE_DIR}/.Ini/globalCompiling.ini ${CMAKE_BINARY_DIR}/.Ini/globalCompiling.ini COPYONLY)

# Opening the subdirectory for the next works
add_subdirectory(Models)
add_subdirectory(Sources)
add_subdirectory(Tests)
