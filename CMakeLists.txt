cmake_minimum_required(VERSION 3.12)
project(baldur)

set(CMAKE_CXX_STANDARD 17)

# add slang library
add_subdirectory(extern/slang)
add_subdirectory(extern/googletest/)

set(CMAKE_BUILD_TYPE Debug)
# add main library
add_subdirectory(src)

# tests
include(GoogleTest)
include(CTest)
enable_testing()
add_subdirectory(tests)