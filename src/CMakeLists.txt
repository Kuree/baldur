add_library(baldur sv.cc)

target_link_libraries(baldur slang)
target_include_directories(baldur PRIVATE ../extern/slang/include)