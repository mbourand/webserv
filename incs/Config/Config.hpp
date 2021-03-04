#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "ServerConfig.hpp"
#include <list>

class Config
{
	private:
		std::list<ServerConfig> _servers;

	public:
		Config();
		Config(const Config& other);
		Config(const std::string& raw);
		virtual ~Config();
		Config& operator=(const Config& other);

		std::list<ServerConfig>& getServers();
};

#endif
