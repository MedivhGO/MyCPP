cmake_minimum_required(VERSION 3.16)
project(MyCpp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(MYCPP_ENABLE_ASAN "Build with AddressSanitizer" OFF)
if (MYCPP_ENABLE_ASAN)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address)
endif()

include(CTest)

add_subdirectory(src)
if (BUILD_TESTING)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    add_subdirectory(third_party/googletest)
    add_subdirectory(test)
endif()