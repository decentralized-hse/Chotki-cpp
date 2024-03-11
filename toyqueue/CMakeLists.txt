set(LIB_TARGET toyqueue)

option(TOYQUEUE_EXAMPLES "Build toyqueue examples" OFF)
option(TOYQUEUE_TESTS "Build toyqueue tests" OFF)

get_filename_component(LIB_INCLUDE_PATH "include" ABSOLUTE)
get_filename_component(LIB_PATH "src" ABSOLUTE)

file(GLOB_RECURSE LIB_HEADERS ${LIB_INCLUDE_PATH}/*.hpp)
file(GLOB_RECURSE LIB_CXX_SOURCES ${LIB_PATH}/*.cpp)

Log("Toy Queue headers: ${LIB_HEADERS}")
Log("Toy Queue sources: ${LIB_CXX_SOURCES}")

add_library(${LIB_TARGET} STATIC ${LIB_HEADERS} ${LIB_CXX_SOURCES})
target_include_directories(${LIB_TARGET} PUBLIC ${LIB_INCLUDE_PATH})

if (TOYQUEUE_EXAMPLES)
    add_subdirectory(examples)
endif()

if (TOYQUEUE_TESTS)
    add_subdirectory(tests)
endif()
