#ifndef LOCATION_CONFIG_HPP
#define LOCATION_CONFIG_HPP

#include <map>
#include <string>

class LocationConfig
{
	private:
		std::map<std::string, std::string> _params;

	public:
		LocationConfig();
		LocationConfig(const LocationConfig& other);
		LocationConfig(const std::string& name, const std::string& raw);
		virtual ~LocationConfig();
		LocationConfig& operator=(const LocationConfig& other);
};

#endif
