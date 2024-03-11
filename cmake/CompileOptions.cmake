# Common compile options for C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-g -fno-omit-frame-pointer)
add_compile_options(-gdwarf-4)

Log("C++ standard: ${CMAKE_CXX_STANDARD}")
