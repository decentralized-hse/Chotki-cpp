function(MakeExample lib main_file)
    set(target_example ${lib}_${main_file})
    Log("Add example: ${target_example}")

    get_filename_component(EXAMPLE_PATH "." ABSOLUTE)
    file(GLOB_RECURSE EXAMPLE_CXX_SOURCES ${EXAMPLE_PATH}/${main_file}.cpp)

    add_executable(${target_example} ${EXAMPLE_CXX_SOURCES})
    target_link_libraries(${target_example} ${lib})
endfunction()
