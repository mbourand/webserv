#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <string>
#include <map>
#include <list>
#include "LocationConfig.hpp"

class ServerConfig
{
	private:
		std::map<std::string, std::list<std::string> > _params;
		std::list<LocationConfig> _locations;
		std::map<int, std::string> _error_pages;
		std::list<std::string> _names;
		std::list<int> _ports;

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

		const std::map<std::string, std::list<std::string> >& getParams() const;
		const std::map<int, std::string>& getErrorPages() const;
		const std::list<std::string>& getNames() const;
		const std::list<int>& getPorts() const;
};

#endif
