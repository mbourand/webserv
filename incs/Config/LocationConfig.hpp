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

		std::string getToken(const std::string& val, size_t token);
		int getInt(const std::string& param, size_t token);
		std::string getString(const std::string& param, size_t token);
		std::string getPath();
};

#endif
