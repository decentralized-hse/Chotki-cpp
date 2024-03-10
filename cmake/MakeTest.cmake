function(MakeTest lib)
    set(test_case test_${lib})
    Log("Add test case: ${test_case}")

    get_filename_component(TEST_PATH "." ABSOLUTE)
    file(GLOB_RECURSE TEST_CXX_SOURCES ${TEST_PATH}/*.cpp)
    Log("Add test files: ${TEST_CXX_SOURCES}")

    add_executable(${test_case} ${TEST_CXX_SOURCES})
    target_link_libraries(${test_case} ${lib} GTest::gtest_main)
    add_test(NAME ${test_case} COMMAND ${test_case})
endfunction()
