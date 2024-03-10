cmake_minimum_required(VERSION 3.16)

project(chotki)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/Logging.cmake)
include(cmake/CCache.cmake)
include(cmake/CheckPlatform.cmake)
include(cmake/CheckCompiler.cmake)
include(cmake/CompileOptions.cmake)

add_subdirectory(third_party)

add_subdirectory(toykv)
Log("Toykv include: ${toykv_INCLUDE}")

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${toykv_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE seastar)
target_link_libraries(${PROJECT_NAME} PRIVATE rocksdb)
