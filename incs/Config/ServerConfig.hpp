#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <string>
#include <map>
#include <list>
#include "LocationConfig.hpp"

#define DEF_SERVER_NAME "localhost"
#define DEF_SERVER_PORT "8080"
#define DEF_CLIENT_MAX_BODY "8000"
#define DEF_ROOT "/webserv/www/"

class ServerConfig
{
	private:
		std::map<std::string, std::string> _params;
		std::list<LocationConfig> _locations;
		std::map<int, std::string> _error_pages;

	public:
		ServerConfig();
		ServerConfig(const ServerConfig& other);
		ServerConfig(const std::string& raw);
		virtual ~ServerConfig();
		ServerConfig& operator=(const ServerConfig& other);

		std::string getToken(const std::string& str, size_t token);
		int getInt(const std::string& param, size_t token);
		std::string getString(const std::string& param, size_t token);
		std::list<LocationConfig>& getLocations();
		std::string getErrorPage(int code);
		std::string getErrorMessage(int code);
		std::map<std::string, std::string>& getParams();
		std::map<int, std::string>& getErrorPages();
};

#endif
