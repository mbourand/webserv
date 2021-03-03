/*
** server_name
** port
** Le 1er serveur pour host:port est celui par défaut
** Pages d'erreurs par défaut
** Taille max du body client
** location
**    liste de methods activées
**    root
**    directory_listing
**    fichier par défaut si la requête est un dossier
**    tous les trucs cgi a voir plus tard
**    choisir le dossier où mettre les fichiers uploadés
*/

#include "ServerConfig.hpp"

ServerConfig::ServerConfig()
{
	_params["server_name"] = DEF_SERVER_NAME;
	_params["listen"] = DEF_SERVER_PORT;
	_params["client_max_body"] = DEF_CLIENT_MAX_BODY;
}

ServerConfig::~ServerConfig()
{}

ServerConfig::ServerConfig(const ServerConfig& other)
{
	*this = other;
}

ServerConfig& ServerConfig::operator=(const ServerConfig& other)
{
	_params = other._params;
	return *this;
}

ServerConfig::ServerConfig(const std::string& raw)
{
	_params["server_name"] = DEF_SERVER_NAME;
	_params["listen"] = DEF_SERVER_PORT;
	_params["client_max_body"] = DEF_CLIENT_MAX_BODY;

	for (size_t i = 0; i < raw.size();)
	{
		while (raw[i] == ' ' || raw[i] == '\t')
			i++;
		if (raw[i] == '#')
		{
			i = raw.find('\n', i) + 1;
			continue;
		}
		if (raw.find_first_of(" \t", i) > raw.find('\n', i))
			throw std::invalid_argument("Invalid config file");

		std::string directive_name = raw.substr(i, raw.find_first_of(" \t", i) - i);
		i += directive_name.size();
		while (raw[i] == ' ' || raw[i] == '\t')
			i++;

		if (directive_name == "location")
		{
			std::string location_name = raw.substr(i, raw.find_first_of(" \t", i) - i);
			i += location_name.size();
			while (raw[i] == ' ' || raw[i] == '\t')
				i++;
			if (raw[i] != '{')
				throw std::invalid_argument("Invalid config file");
			std::string location_raw = raw.substr(i, raw.find("}", i) - i - 1);
			_locations.push_back(LocationConfig(location_name, location_raw));
		}

		std::string directive_value = raw.substr(i, raw.find('\n', i) - i);
		if (directive_value != "{")
			_params.insert(std::make_pair(directive_name, directive_value));
		i = raw.find('\n', i) + 1;
	}
}

