cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CXX_TEST_CODE "std::counting_semaphore<1> sem(1);")
#check_cxx_source_compiles("$(XCC_TEST_CODE" HAS_CXX20_semaphore)
#if (!HAS_CXX20_semaphore)
#  message("Compiler does not support C++20 semaphore")
#endif()

add_executable(
  ${PROJECT_NAME}.test
  AllTests.cpp
  test_mystr.cpp
  test_mysp.cpp
  test_mymat.cpp
  test_myutil.cpp
  test_myds.cpp
  test_cppfeature.cpp
  test_mysort.cpp
  test_mybst.cpp
  test_myvec.cpp
  test_myskiplist.cpp
  test_topn.cpp
  test_mock.cpp
  FixedThreadPoolTest.cpp
  test_concurrency.cpp
  test_memorypool.cpp)

target_link_libraries(${PROJECT_NAME}.test ${PROJECT_NAME} gtest pthread gmock) 

target_compile_definitions(${PROJECT_NAME}.test
                           PRIVATE TEST_DIR="${CMAKE_CURRENT_LIST_DIR}/test")

install(TARGETS ${PROJECT_NAME}.test DESTINATION "${tool_dest}")

# make single test
#add_executable(test_mysp.test AllTests.cpp test_mysp.cpp)

#add_executable(test_mymat.test AllTests.cpp test_mymat.cpp)

#target_link_libraries(test_mysp.test ${PROJECT_NAME} gtest pthread)

#target_link_libraries(test_mymat.test ${PROJECT_NAME} gtest pthread)

set(MAIN_PATH ${PROJECT_SOURCE_DIR})
message(STATUS ${MAIN_PATH})
configure_file(${PROJECT_SOURCE_DIR}/resource/MyProjectPath.h.in
               ${PROJECT_SOURCE_DIR}/resource/MyProjectPath.h)
