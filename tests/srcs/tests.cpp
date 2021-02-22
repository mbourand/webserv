#include <iostream>
#include <gtest/gtest.h>
#include <random>
#include <dirent.h>
#include <fstream>
#include "Request.hpp"

static void request_from_file(const std::string& filename, size_t buf_size)
{
	std::fstream file(filename);

	Request req;
	char* buf = new char[buf_size + 1];

	bzero(buf, buf_size + 1);
	while (file.read(buf, buf_size))
	{
		req.append(buf);
		bzero(buf, buf_size + 1);
	}
	req.append(buf);
	delete[] buf;
	file.close();
}

TEST(request_parser, bad_whitespaces)
{
	for (int i = 1; i <= 1024; i = (i << 1))
	{
		ASSERT_ANY_THROW(request_from_file("request_parser/bad_whitespaces/invalid_bad_whitespace1", i));
		ASSERT_ANY_THROW(request_from_file("request_parser/bad_whitespaces/invalid_bad_whitespace2", i));
		ASSERT_ANY_THROW(request_from_file("request_parser/bad_whitespaces/invalid_bad_whitespace3", i));
		ASSERT_ANY_THROW(request_from_file("request_parser/bad_whitespaces/invalid_bad_whitespace4", i));
	}
}


TEST(request_parser, case_sensitive)
{
	for (int i = 1; i <= 1024; i = (i << 1))
	{
		ASSERT_ANY_THROW(request_from_file("request_parser/case_sensitive/invalid_case_sensitive1", i));
		ASSERT_ANY_THROW(request_from_file("request_parser/case_sensitive/invalid_case_sensitive2", i));
		ASSERT_ANY_THROW(request_from_file("request_parser/case_sensitive/invalid_case_sensitive3", i));
		ASSERT_ANY_THROW(request_from_file("request_parser/case_sensitive/invalid_case_sensitive4", i));
	}
}

TEST(request_parser, crlf)
{
	for (int i = 1; i < 5; i = (i << 1))
	{
		ASSERT_ANY_THROW(request_from_file("request_parser/CRLF/invalid_CRLF", i));
		ASSERT_ANY_THROW(request_from_file("request_parser/CRLF/invalid_CRLFCRLF", i));
	}
}


TEST(request_parser, header_delimiter)
{
	for (int i = 1; i <= 1024; i = (i << 1))
	{
		ASSERT_ANY_THROW(request_from_file("request_parser/header_delimiter/invalid_header_delimiter1", i));
		ASSERT_ANY_THROW(request_from_file("request_parser/header_delimiter/invalid_header_delimiter2", i));
		ASSERT_ANY_THROW(request_from_file("request_parser/header_delimiter/invalid_header_delimiter3", i));
		ASSERT_NO_THROW(request_from_file("request_parser/header_delimiter/valid_header_delimiter1", i));
		ASSERT_NO_THROW(request_from_file("request_parser/header_delimiter/valid_header_delimiter2", i));
	}
}

TEST(request_parser, missing_parts)
{
	for (int i = 1; i <= 1024; i = (i << 1))
	{
		ASSERT_ANY_THROW(request_from_file("request_parser/missing_parts/invalid_no_header", i));
		ASSERT_ANY_THROW(request_from_file("request_parser/missing_parts/invalid_no_header_no_body", i));
		ASSERT_NO_THROW(request_from_file("request_parser/missing_parts/valid_no_body", i));
	}
}

TEST(request_parser, simple_valid_case)
{
	for (size_t i = 1; i <= 1024; i = (i << 1))
	{
		ASSERT_NO_THROW(request_from_file("request_parser/valid/valid_1", i));
	}
}

int main(int argc, char **argv)
{
	Logger::setMode(SILENT);
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
