cmake_minimum_required(VERSION 3.12)
project(baldur)

set(CMAKE_CXX_STANDARD 20)

# add slang library
add_subdirectory(extern/slang)
add_subdirectory(extern/googletest/)

set(CMAKE_CXX_FLAGS "-O0 -ggdb -g")

# add main library
add_subdirectory(src)

# tests
include(GoogleTest)
include(CTest)
enable_testing()
add_subdirectory(tests)