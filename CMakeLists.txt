cmake_minimum_required(VERSION 3.12)
project(baldur)

set(CMAKE_CXX_STANDARD 20)

# add slang library
add_subdirectory(extern/slang)

# add main library
add_subdirectory(src)