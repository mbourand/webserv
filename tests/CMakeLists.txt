cmake_minimum_required(VERSION 3.10.2) # version can be different
project(webserv) #name of your project
add_subdirectory(googletest) # add googletest subdirectory
include_directories(googletest/include) # this is so we can #include <gtest/gtest.h>

set(INCLUDES ../incs)
set(SOURCES ../srcs)
set(SOURCES_LIST
	srcs/tests.cpp
	${INCLUDES}/Methods/ConnectMethod.hpp
	${INCLUDES}/Methods/DeleteMethod.hpp
	${INCLUDES}/Methods/GetMethod.hpp
	${INCLUDES}/Methods/HeadMethod.hpp
	${INCLUDES}/Methods/OptionsMethod.hpp
	${INCLUDES}/Methods/PostMethod.hpp
	${INCLUDES}/Methods/PutMethod.hpp
	${INCLUDES}/Methods/TraceMethod.hpp
	${INCLUDES}/Methods/MethodSource.hpp
	${INCLUDES}/Methods/IMethod.hpp
	${INCLUDES}/Utils/Logger.hpp
	${INCLUDES}/Request.hpp
	${SOURCES}/Methods/ConnectMethod.cpp
	${SOURCES}/Methods/DeleteMethod.cpp
	${SOURCES}/Methods/GetMethod.cpp
	${SOURCES}/Methods/HeadMethod.cpp
	${SOURCES}/Methods/OptionsMethod.cpp
	${SOURCES}/Methods/PostMethod.cpp
	${SOURCES}/Methods/PutMethod.cpp
	${SOURCES}/Methods/TraceMethod.cpp
	${SOURCES}/Methods/MethodSource.cpp
	${SOURCES}/Utils/Logger.cpp
	${SOURCES}/Request.cpp
)
add_executable(tester ${SOURCES_LIST}) # add this executable
target_link_libraries(tester PRIVATE gtest) # link google test to this executable
target_include_directories(tester PRIVATE ${INCLUDES} PRIVATE ${INCLUDES}/Utils PRIVATE ${INCLUDES}/Methods)
