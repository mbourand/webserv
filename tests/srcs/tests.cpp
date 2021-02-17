#include <iostream>
#include <gtest/gtest.h>
#include <random>
#include <dirent.h>
#include <fstream>
#include "Request.hpp"

static std::string read_file(const char *name)
{
	char buffer[1024];
	bzero(buffer, 1024);
	std::ifstream file(name);
	std::string content;
	while (file.read(buffer, 1024))
	{
		content += buffer;
		bzero(buffer, 1024);
	}
	content += buffer;
	return content;
}

TEST(request_parser, bad_whitespaces)
{
	ASSERT_ANY_THROW(Request req(read_file("request_parser/bad_whitespaces/invalid_bad_whitespace1")));
	ASSERT_ANY_THROW(Request req(read_file("request_parser/bad_whitespaces/invalid_bad_whitespace2")));
	ASSERT_ANY_THROW(Request req(read_file("request_parser/bad_whitespaces/invalid_bad_whitespace3")));
	ASSERT_ANY_THROW(Request req(read_file("request_parser/bad_whitespaces/invalid_bad_whitespace7")));
	ASSERT_ANY_THROW(Request req(read_file("request_parser/bad_whitespaces/invalid_bad_whitespace8")));
}

TEST(request_parser, case_sensitive)
{
	ASSERT_ANY_THROW(Request req(read_file("request_parser/case_sensitive/invalid_case_sensitive1")));
	ASSERT_ANY_THROW(Request req(read_file("request_parser/case_sensitive/invalid_case_sensitive2")));
	ASSERT_ANY_THROW(Request req(read_file("request_parser/case_sensitive/invalid_case_sensitive3")));
	ASSERT_ANY_THROW(Request req(read_file("request_parser/case_sensitive/invalid_case_sensitive4")));
}

TEST(request_parser, crlf)
{
	ASSERT_ANY_THROW(Request req(read_file("request_parser/CRLF/invalid_CRLF")));
	ASSERT_ANY_THROW(Request req(read_file("request_parser/CRLF/invalid_CRLFCRLF")));
	ASSERT_ANY_THROW(Request req(read_file("request_parser/CRLF/invalid_empty")));
}

TEST(request_parser, header_delimiter)
{
	ASSERT_ANY_THROW(Request req(read_file("request_parser/header_delimiter/invalid_header_delimiter1")));
	ASSERT_ANY_THROW(Request req(read_file("request_parser/header_delimiter/invalid_header_delimiter2")));
	ASSERT_ANY_THROW(Request req(read_file("request_parser/header_delimiter/invalid_header_delimiter3")));
	ASSERT_NO_THROW(Request req(read_file("request_parser/header_delimiter/valid_header_delimiter1")));
	ASSERT_NO_THROW(Request req(read_file("request_parser/header_delimiter/valid_header_delimiter2")));
}

TEST(request_parser, missing_parts)
{
	ASSERT_ANY_THROW(Request req(read_file("request_parser/missing_parts/invalid_no_header")));
	ASSERT_ANY_THROW(Request req(read_file("request_parser/missing_parts/invalid_no_header_no_body")));
	ASSERT_NO_THROW(Request req(read_file("request_parser/missing_parts/valid_no_body")));
}

int main(int argc, char **argv)
{
	Logger::setMode(VERBOSE);
	testing::InitGoogleTest(&argc, argv);
	
	return RUN_ALL_TESTS();
}
