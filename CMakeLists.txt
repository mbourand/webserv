cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(webserv VERSION 1.0)
file(GLOB_RECURSE SRC_FILES srcs/*.cpp)
add_executable(webserv ${SRC_FILES})


target_include_directories(webserv PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/incs/Config)
target_include_directories(webserv PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/incs/Headers)
target_include_directories(webserv PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/incs/Methods)
target_include_directories(webserv PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/incs/Socket)
target_include_directories(webserv PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/incs/Utils)
target_include_directories(webserv PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/incs/)


find_library(PTHREAD pthread)
target_link_libraries(webserv PUBLIC ${PTHREAD})
find_library(ZLIB z)
target_link_libraries(webserv PUBLIC ${ZLIB})
