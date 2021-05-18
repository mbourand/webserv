cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME webserv)

project(${PROJECT_NAME} VERSION 1.0)
file(GLOB_RECURSE SRC_FILES srcs/*.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})


target_include_directories(${PROJECT_NAME} PUBLIC	${CMAKE_CURRENT_SOURCE_DIR}/incs/Config	${CMAKE_CURRENT_SOURCE_DIR}/incs/Headers
													${CMAKE_CURRENT_SOURCE_DIR}/incs/Methods ${CMAKE_CURRENT_SOURCE_DIR}/incs/Socket
													${CMAKE_CURRENT_SOURCE_DIR}/incs/Utils ${CMAKE_CURRENT_SOURCE_DIR}/incs)


find_library(PTHREAD pthread)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PTHREAD})

find_library(ZLIB z)
target_link_libraries(${PROJECT_NAME} PUBLIC ${ZLIB})
