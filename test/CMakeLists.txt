add_executable(
  ${PROJECT_NAME}.test
  AllTests.cpp
  container/test_myvec.cpp
  container/test_mystr.cpp
  container/test_mybst.cpp
  container/test_myds.cpp
  container/test_myskiplist.cpp
  memory/test_mysp.cpp
  memory/test_memorypool.cpp
  thread/FixedThreadPoolTest.cpp
  thread/test_concurrency.cpp
  sort/test_mysort.cpp
  util/test_myutil.cpp
  util/test_mymat.cpp
  util/test_topn.cpp
  util/test_mock.cpp
  util/test_cppfeature.cpp)

target_link_libraries(${PROJECT_NAME}.test PRIVATE ${PROJECT_NAME} GTest::gmock)

set(MAIN_PATH ${PROJECT_SOURCE_DIR})
configure_file(
    ${CMAKE_SOURCE_DIR}/resource/MyProjectPath.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated/MyProjectPath.h
)
target_include_directories(${PROJECT_NAME}.test PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}.test)

# 单独编译单个测试文件（示例）：
#   cmake --build build --target test_myskiplist.test
#   ./build/test/test_myskiplist.test
add_executable(test_myskiplist.test AllTests.cpp container/test_myskiplist.cpp)
target_link_libraries(test_myskiplist.test PRIVATE ${PROJECT_NAME} GTest::gmock)
