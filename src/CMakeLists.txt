add_library(baldur sv.cc symbol.cc util.cc)

target_link_libraries(baldur slangcompiler)
target_include_directories(baldur PRIVATE ../extern/slang/include
                                          ../extern/fmt/include)