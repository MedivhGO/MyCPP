set(CMAKE_CXX_STANDARD 20)
set(header_path "${${PROJECT_NAME}_SOURCE_DIR}/include")
set(header ${header_path}/MyString.h)
set(src MyString.cpp)

set(head_bst ${header_path}/MyBST.h)
set(src_bst MyBST.cpp)

set(head_threadpool ${header_path}/FixedThreadPool.h)
set(src_threadpool FixedThreadPool.cpp)

add_library(${PROJECT_NAME} SHARED ${header} ${src} ${head_bst} ${src_bst} ${head_threadpool} ${src_threadpool})

target_include_directories(
  ${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
                         ${${PROJECT_NAME}_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} pthread)

add_executable(${PROJECT_NAME}.info main.cpp)
target_include_directories(
  ${PROJECT_NAME}.info PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
                              ${${PROJECT_NAME}_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME}.info ${PROJECT_NAME})

install(
  TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION "${lib_dest}"
  ARCHIVE DESTINATION "${lib_dest}" COMPONENT library)
install(TARGETS ${PROJECT_NAME}.info RUNTIME DESTINATION "${tool_dest}"
                                             COMPONENT library)
install(FILES ${header} DESTINATION "${include_dest}")
