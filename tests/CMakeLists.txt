cmake_minimum_required(VERSION 3.10.2) # version can be different
project(webserv) #name of your project
add_subdirectory(googletest) # add googletest subdirectory
include_directories(googletest/include) # this is so we can #include <gtest/gtest.h>

set(INCLUDES ../incs)
set(SOURCES ../srcs)
set(SOURCES_LIST
	srcs/tests.cpp
	${INCLUDES}/Headers/AcceptCharsetsHeader.hpp
	${INCLUDES}/Headers/AcceptLanguageHeader.hpp
	${INCLUDES}/Headers/AllowHeader.hpp
	${INCLUDES}/Headers/AuthorizationHeader.hpp
	${INCLUDES}/Headers/ContentLanguageHeader.hpp
	${INCLUDES}/Headers/ContentLengthHeader.hpp
	${INCLUDES}/Headers/ContentLocationHeader.hpp
	${INCLUDES}/Headers/ContentTypeHeader.hpp
	${INCLUDES}/Headers/DateHeader.hpp
	${INCLUDES}/Headers/HostHeader.hpp
	${INCLUDES}/Headers/LastModifiedHeader.hpp
	${INCLUDES}/Headers/LocationHeader.hpp
	${INCLUDES}/Headers/RefererHeader.hpp
	${INCLUDES}/Headers/RetryAfterHeader.hpp
	${INCLUDES}/Headers/ServerHeader.hpp
	${INCLUDES}/Headers/TransferEncodingHeader.hpp
	${INCLUDES}/Headers/Header.hpp
	${INCLUDES}/Headers/UserAgentHeader.hpp
	${INCLUDES}/Headers/WWWAuthenticateHeader.hpp
	${INCLUDES}/Methods/ConnectMethod.hpp
	${INCLUDES}/Methods/DeleteMethod.hpp
	${INCLUDES}/Methods/GetMethod.hpp
	${INCLUDES}/Methods/HeadMethod.hpp
	${INCLUDES}/Methods/OptionsMethod.hpp
	${INCLUDES}/Methods/PostMethod.hpp
	${INCLUDES}/Methods/PutMethod.hpp
	${INCLUDES}/Methods/TraceMethod.hpp
	${INCLUDES}/Methods/IMethod.hpp
	${INCLUDES}/Utils/Factory.hpp
	${INCLUDES}/Utils/Logger.hpp
	${INCLUDES}/Request.hpp
	${SOURCES}/Headers/AcceptCharsetsHeader.cpp
	${SOURCES}/Headers/AcceptLanguageHeader.cpp
	${SOURCES}/Headers/AllowHeader.cpp
	${SOURCES}/Headers/AuthorizationHeader.cpp
	${SOURCES}/Headers/ContentLanguageHeader.cpp
	${SOURCES}/Headers/ContentLengthHeader.cpp
	${SOURCES}/Headers/ContentLocationHeader.cpp
	${SOURCES}/Headers/ContentTypeHeader.cpp
	${SOURCES}/Headers/DateHeader.cpp
	${SOURCES}/Headers/HostHeader.cpp
	${SOURCES}/Headers/LastModifiedHeader.cpp
	${SOURCES}/Headers/LocationHeader.cpp
	${SOURCES}/Headers/RefererHeader.cpp
	${SOURCES}/Headers/RetryAfterHeader.cpp
	${SOURCES}/Headers/ServerHeader.cpp
	${SOURCES}/Headers/TransferEncodingHeader.cpp
	${SOURCES}/Headers/UserAgentHeader.cpp
	${SOURCES}/Headers/WWWAuthenticateHeader.cpp
	${SOURCES}/Headers/Header.cpp
	${SOURCES}/Methods/ConnectMethod.cpp
	${SOURCES}/Methods/DeleteMethod.cpp
	${SOURCES}/Methods/GetMethod.cpp
	${SOURCES}/Methods/HeadMethod.cpp
	${SOURCES}/Methods/OptionsMethod.cpp
	${SOURCES}/Methods/PostMethod.cpp
	${SOURCES}/Methods/PutMethod.cpp
	${SOURCES}/Methods/TraceMethod.cpp
	${SOURCES}/Utils/Logger.cpp
	${SOURCES}/Request.cpp
)
add_executable(tester ${SOURCES_LIST}) # add this executable
target_link_libraries(tester PRIVATE gtest) # link google test to this executable
target_include_directories(tester PRIVATE ${INCLUDES} PRIVATE ${INCLUDES}/Utils PRIVATE ${INCLUDES}/Methods PRIVATE ${INCLUDES}/Headers)
