set(LIB_TARGET toykv)

option(TOYKV_EXAMPLES "Build toykv examples" OFF)
option(TOYKV_TESTS "Build toykv tests" OFF)

get_filename_component(LIB_INCLUDE_PATH "include" ABSOLUTE)
get_filename_component(LIB_PATH "src" ABSOLUTE)

file(GLOB_RECURSE LIB_HEADERS ${LIB_INCLUDE_PATH}/*.hpp)
file(GLOB_RECURSE LIB_CXX_SOURCES ${LIB_PATH}/*.cpp)

Log("Toy KV headers: ${LIB_HEADERS}")
Log("Toy KV sources: ${LIB_CXX_SOURCES}")

add_library(${LIB_TARGET} STATIC ${LIB_HEADERS} ${LIB_CXX_SOURCES})
target_include_directories(${LIB_TARGET} PUBLIC ${LIB_INCLUDE_PATH})

set(LIB_DEPENDENCIES rocksdb)
target_link_libraries(${LIB_TARGET} PUBLIC ${LIB_DEPENDENCIES})

if (TOYKV_EXAMPLES)
    add_subdirectory(examples)
endif()

if (TOYKV_TESTS)
    add_subdirectory(tests)
endif()
