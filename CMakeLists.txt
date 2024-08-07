cmake_minimum_required(VERSION 3.10.2)
project(MyCpp)

set(GOOGLETEST_VERSION 1.11.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address
 -ASAN_OPTIONS=detect_odr_violation=0 -std=c++20")
set(CMAKE_CXX_FLAGS "-DCMAKE_EXPORT_COMPILE_COMMANDS=1")

message(STATUS "Begin CmakeList")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID} version ${CMAKE_CXX_COMPILER_VERSION}")

set(tool_dest "bin")
set(lib_dest "lib")
set(include_dest "include/")

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(third_party/googletest)

message(STATUS "End CmakeList")
