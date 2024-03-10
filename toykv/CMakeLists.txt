set(LIB_TARGET toykv)

option(BUILD_EXAMPLES "Build executable" OFF)

get_filename_component(LIB_INCLUDE_PATH "./include" ABSOLUTE)
get_filename_component(LIB_PATH "./src" ABSOLUTE)

file(GLOB_RECURSE LIB_HEADERS ${LIB_INCLUDE_PATH}/*.hpp)
file(GLOB_RECURSE LIB_CXX_SOURCES ${LIB_PATH}/*.cpp)

Log("Toy KV headers: ${LIB_HEADERS}")
Log("Toy KV sources: ${LIB_CXX_SOURCES}")

add_library(${LIB_TARGET} STATIC ${LIB_HEADERS} ${LIB_CXX_SOURCES})

set(LIB_DEPENDENCIES rocksdb)
target_link_libraries(${LIB_TARGET} PUBLIC rocksdb)

include_directories(${LIB_TARGET} PUBLIC ${LIB_INCLUDE_PATH})

if (BUILD_BIN)
    add_subdirectory(examples)
endif()

set(toykv_INCLUDE ${LIB_INCLUDE_PATH} PARENT_SCOPE)
set(toykv_SOURCES ${LIB_CXX_SOURCES} PARENT_SCOPE)
