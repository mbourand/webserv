#include <map>
#include <iostream>
#include "Logger.hpp"
#include "Request.hpp"
#include <strings.h>
#include <fstream>

int main()
{
	Logger::setMode(VERBOSE);
	try
	{
		for (size_t buff_size = 1; buff_size < 500; buff_size = (buff_size << 1))
		{
			Request req;
			char* buf = new char[buff_size + 1];
			bzero(buf, buff_size + 1);
			std::fstream file("tests/request_parser/valid/valid_1");
			while (file.read(buf, buff_size))
			{
				req.append(std::string(buf));
				bzero(buf, buff_size + 1);
			}
			req.append(buf);
			file.close();
			Logger::print("Success", NULL, SUCCESS, NORMAL);
			delete[] buf;
		}
	}
	catch(const std::exception& e)
	{
		Logger::print(e.what(), NULL, ERROR, NORMAL);
	}
}
