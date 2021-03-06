#include <stdexcept>
#include <iostream>
#include <list>
#include "VirtualHost.hpp"
#include <fstream>

void init_config(const std::string& filename, std::list<VirtualHost>& vhosts)
{
	std::ifstream file(filename.c_str(), std::ifstream::in);
	if (!file.good() || !file.is_open())
		throw std::invalid_argument("Couldn't open configuration file");

	std::string raw((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

	for (size_t i = 0; i < raw.size();)
	{
		while (raw[i] == ' ' || raw[i] == '\t')
			i++;
		if (raw[i] == '\n' || raw[i] == '#')
		{
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
				throw std::invalid_argument("Invalid config file");

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
			vhosts.push_back(VirtualHost(ServerConfig(raw.substr(start_i, i - start_i))));
			i = raw.find('\n', i) + 1;
		}
	}
}
