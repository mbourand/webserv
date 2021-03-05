#include "Config.hpp"
#include <stdexcept>
#include <iostream>

/*
** -------------------------------- CONSTRUCTOR ------------------------------
*/

Config::Config()
{}

Config::~Config()
{}

Config::Config(const Config& other)
{
	*this = other;
}

Config::Config(const std::string& raw)
{
	for (size_t i = 0; i < raw.size();)
	{
		while (raw[i] == ' ' || raw[i] == '\t')
			i++;
		if (raw[i] == '\n')
		{
			i++;
			continue;
		}
		if (raw[i] == '#')
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
				throw std::invalid_argument("Config: Constructor: Invalid config file");

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
			_servers.push_back(ServerConfig(raw.substr(start_i, i - start_i)));
			i += 2;
		}
	}
}

/*
** -------------------------------- OPERATOR ------------------------------
*/

Config& Config::operator=(const Config& other)
{
	_servers = other._servers;
	return *this;
}

/*
** -------------------------------- ACCESSORS ------------------------------
*/

std::list<ServerConfig>& Config::getServers()
{
	return _servers;
}

ServerConfig& Config::getServer(const std::string& uri)
{
	std::string domain = uri;
	if (domain.substr(0, 7) == "http://")
		domain = domain.substr(7);

	if (domain[0] == '/')
		throw std::invalid_argument("Config: GetServer: Bad URI");
	domain = domain.substr(domain.find('/') - 1);

	for (std::list<ServerConfig>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		size_t token = 0;
		try
		{
			while (42)
				if (it->getString("server_name", token++) == domain)
					return *it;
		}
		catch (const std::exception& e)
		{}
	}
	throw std::invalid_argument("Config: GetServer: No server_name compatible with uri");
}
