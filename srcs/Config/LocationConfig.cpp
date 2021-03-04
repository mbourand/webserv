#include "LocationConfig.hpp"
#include "Logger.hpp"

LocationConfig::LocationConfig()
{
	_params["path"] = "/";
	_params[""] = "";
	_params[""] = "";
	_params[""] = "";
	_params[""] = "";
	_params[""] = "";
}

LocationConfig::~LocationConfig()
{}

LocationConfig::LocationConfig(const LocationConfig& other)
{
	*this = other;
}

LocationConfig& LocationConfig::operator=(const LocationConfig& other)
{
	_params = other._params;
	return *this;
}

LocationConfig::LocationConfig(const std::string& path, const std::string& raw)
{
	_params["path"] = path;
}
