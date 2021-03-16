#include <stdexcept>
#include <iostream>
#include <list>
#include "VirtualHost.hpp"
#include <fstream>
#include "Logger.hpp"
#include <algorithm>

void init_config(const std::string& filename, std::list<VirtualHost>& vhosts)
{
	std::ifstream file(filename.c_str(), std::ifstream::in);
	if (!file.good() || !file.is_open())
		throw std::invalid_argument("Couldn't open configuration file");

	std::string raw((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

	if (std::count(raw.begin(), raw.end(), '{') != std::count(raw.begin(), raw.end(), '}'))
		throw std::invalid_argument("Bad braces in config.");

	for (size_t i = 0; i < raw.size();)
	{
		while (raw[i] == ' ' || raw[i] == '\t')
			i++;
		if (raw[i] == '\n' || raw[i] == '#')
		{
			if (raw.find('\n', i) == std::string::npos)
				break;
			i = raw.find('\n', i) + 1;
			continue;
		}
		if (raw.find("server", i) == i)
		{
			i += 6;
			while (raw[i] == ' ' || raw[i] == '\t')
				i++;
			if (raw[i] == '{' && raw[i + 1] == '\n')
				i += 2;
			else
				throw std::invalid_argument("Expected newline after { in config");

			size_t nesting = 1;
			size_t start_i = i;
			while (i < raw.size())
			{
				if (raw[i] == '{')
					nesting++;
				else if (raw[i] == '}')
					nesting--;
				if (nesting == 0)
					break;
				i++;
			}
			if (nesting != 0)
				throw std::invalid_argument("Bad braces in config file");
			if (raw[i + 1] != '\n')
				throw std::invalid_argument("Expected newline after } in config");
			vhosts.push_back(VirtualHost(ConfigContext(raw.substr(start_i, i - start_i))));
			if (raw.find('\n', i) == std::string::npos)
				break;
			i = raw.find('\n', i) + 1;
		}
		else
			throw std::invalid_argument("Bad directive in config");
	}
	Logger::print("Config was parsed successfully", NULL, SUCCESS, NORMAL);
}
