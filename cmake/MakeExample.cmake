function(MakeExample lib main_file)
    Log("Add example: ${main_file}")

    get_filename_component(EXAMPLE_PATH "." ABSOLUTE)
    file(GLOB_RECURSE EXAMPLE_CXX_SOURCES ${EXAMPLE_PATH}/${main_file}.cpp)

    add_executable(${main_file} ${EXAMPLE_CXX_SOURCES})
    target_link_libraries(${main_file} ${lib})
endfunction()