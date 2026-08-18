set(MYCPP_SOURCES
  MyString.cpp
  MyBST.cpp
  FixedThreadPool.cpp
  MySkipList.cpp
  MyLog.cpp)


add_library(${PROJECT_NAME} SHARED ${MYCPP_SOURCES})

find_package(Threads REQUIRED)

target_include_directories(
  ${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads)

add_executable(${PROJECT_NAME}.info main.cpp)
target_link_libraries(${PROJECT_NAME}.info PRIVATE ${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(TARGETS ${PROJECT_NAME}.info RUNTIME DESTINATION bin)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)
