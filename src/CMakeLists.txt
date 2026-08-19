set(MYCPP_SOURCES
  MyString.cpp
  MyBST.cpp
  MyFixedThreadPool.cpp
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

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}.info
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
