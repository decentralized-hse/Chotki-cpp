cmake_minimum_required(VERSION 3.16)

project(chotki)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/Logging.cmake)
include(cmake/CCache.cmake)
include(cmake/CheckPlatform.cmake)
include(cmake/CheckCompiler.cmake)
include(cmake/CompileOptions.cmake)
include(cmake/MakeExample.cmake)
include(cmake/MakeTest.cmake)

add_subdirectory(third_party)
add_subdirectory(toykv)
add_subdirectory(toyqueue)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE toykv toyqueue)
target_link_libraries(${PROJECT_NAME} PRIVATE seastar)
