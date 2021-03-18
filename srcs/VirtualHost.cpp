#include "VirtualHost.hpp"
#include <sstream>
#include <algorithm>
#include <list>
#include "Utils.hpp"
#include "Webserv.hpp"

/*
** ------------------------------- CONSTRUCTOR ------------------------------
*/

VirtualHost::VirtualHost() {}

VirtualHost::VirtualHost(const VirtualHost& other)
{
	*this = other;
}

VirtualHost::VirtualHost(const ConfigContext& config)
	: _config(config)
{
	const std::list<std::string>& listen = _config.getParam("listen");
	for (std::list<std::string>::const_iterator it = listen.begin(); it != listen.end(); it++)
	{
		std::pair<std::string, int> full_ip = ft::complete_ip(*it);
		if (g_webserv.sockets.find(full_ip.second) == g_webserv.sockets.end())
			g_webserv.sockets.insert(std::make_pair(full_ip.second, new ServerSocket(full_ip.second)));
		_ports.push_back(full_ip.second);
	}
}

VirtualHost::~VirtualHost()
{}

/*
** ------------------------------- OPERATOR ------------------------------
*/

VirtualHost& VirtualHost::operator=(const VirtualHost& other)
{
	_config = other._config;
	_ports = other._ports;
	return *this;
}

/*
** ------------------------------- METHODS ------------------------------
*/

std::string VirtualHost::toString() const
{
	return _config.toString();
}

/*
** ------------------------------- ACCESSORS ------------------------------
*/

const ConfigContext& VirtualHost::getConfig() const
{
	return _config;
}

const std::list<int>& VirtualHost::getPorts() const
{
	return _ports;
}

/*
** ------------------------------- STATIC ------------------------------
*/

VirtualHost& VirtualHost::getServerByName(const std::string& name, int port, std::list<VirtualHost>& vhosts)
{
	std::list<VirtualHost*> matches;

	// Cherche le premier serveur avec un port correspondant
	for (std::list<VirtualHost>::iterator vhost_it = vhosts.begin(); vhost_it != vhosts.end(); vhost_it++)
	{
		const std::list<std::string>& listen = vhost_it->getConfig().getParam("listen");
		for (std::list<std::string>::const_iterator listen_it = listen.begin(); listen_it != listen.end(); listen_it++)
		{
			std::pair<std::string, int> complete_listen = ft::complete_ip(*listen_it);
			if ((name == complete_listen.first || complete_listen.first == "0.0.0.0") && port == complete_listen.second)
				matches.push_back(&(*vhost_it));
		}
	}

	if (matches.empty())
		throw std::invalid_argument("No server correspond to request");

	// Si aucun serveur n'a été trouvé, cherche un serveur avec un server_name correspondant
	for (std::list<VirtualHost*>::iterator matches_it = matches.begin(); matches_it != matches.end(); matches_it++)
		if (std::find((*matches_it)->getConfig().getNames().begin(), (*matches_it)->getConfig().getNames().end(), name) != (*matches_it)->getConfig().getNames().end())
			return *(*matches_it);
	return (*matches.front());
}
