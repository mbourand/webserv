#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <string>
#include <map>
#include <list>
#include "LocationConfig.hpp"

#define DEF_SERVER_NAME "localhost"
#define DEF_SERVER_PORT "8080"
#define DEF_CLIENT_MAX_BODY "8000"

class ServerConfig
{
	private:
		std::map<std::string, std::string> _params;
		std::list<LocationConfig> _locations;

	public:
		ServerConfig();
		ServerConfig(const ServerConfig& other);
		ServerConfig(const std::string& raw);
		virtual ~ServerConfig();
		ServerConfig& operator=(const ServerConfig& other);

		int getInt(const std::string& param);
		std::string& getString(const std::string& param);
		std::list<LocationConfig>& getLocations(const std::string& param);
};

#endif
