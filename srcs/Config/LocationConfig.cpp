#include "LocationConfig.hpp"
#include "Logger.hpp"

LocationConfig::LocationConfig()
{
	_params["path"] = "/";
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

std::string LocationConfig::getToken(const std::string& val, size_t token)
{
	size_t actual_token = 0;
	size_t i = 0;
	while (actual_token < token && i < val.size())
	{
		i = val.find_first_of(" \t", i);
		i = val.find_first_not_of(" \t", i);
		actual_token++;
	}
	if (i == val.size())
		throw std::out_of_range("LocationConfig: GetToken: token out of range");
	return val.substr(i, val.find_first_not_of(" \t\n", i) - i);
}

int LocationConfig::getInt(const std::string& param, size_t token)
{
	if (_params.find(param) == _params.end())
		throw std::invalid_argument("LocationConfig: GetInt: Parameter not found");
	std::string token_str = getToken(_params[param], token);
	for (size_t i = 0; i < token_str.size() && token_str[i] != ' ' && token_str[i] != '\t'; i++)
		if (!std::isdigit(token_str[i]))
			throw std::invalid_argument("LocationConfig: GetInt: parameter is not an integer");

	int ret;
	std::istringstream(_params[param]) >> ret;

	return ret;
}

std::string LocationConfig::getString(const std::string& param, size_t token)
{
	if (_params.find(param) == _params.end())
		throw std::invalid_argument("LocationConfig: GetInt: Parameter not found");
	return getToken(_params[param], token);
}

std::string LocationConfig::getPath()
{
	return _params.at("path");
}
