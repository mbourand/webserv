#include <map>
#include <iostream>
#include "Logger.hpp"
#include "Request.hpp"

int main()
{
	std::string raw, line;
	Logger::setMode(VERBOSE);
	while (std::getline(std::cin, line))
		raw += line + '\n';
	try
	{
		Request req;
		req.append(raw);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
