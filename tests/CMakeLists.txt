file(GLOB TEST_VECTORS ${CMAKE_CURRENT_SOURCE_DIR}/vectors/*.sv)
foreach (_test_file ${TEST_VECTORS})
    get_filename_component(_test_name ${_test_file} NAME)
    configure_file(${_test_file}
            ${CMAKE_CURRENT_BINARY_DIR}/${_test_name} COPYONLY)
endforeach ()

function(add_test_target target_name)
    add_executable(${target_name} ${target_name}.cc)
    target_link_libraries(${target_name} gtest baldur gtest_main)
    gtest_discover_tests(${target_name}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vectors)
endfunction()

add_test_target(test_sv)