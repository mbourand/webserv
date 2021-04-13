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
** ------------------------------- ACCESSORS ------------------------------
*/



const ConfigContext& VirtualHost::getConfig() const { return _config; }
const std::list<int>& VirtualHost::getPorts() const { return _ports; }



/*
** ------------------------------- STATIC ------------------------------
*/


/**
 * @brief Retourne le premier VirtualHost de vhosts qui correspond au port et au server_name
 *
 * @param name
 * @param port
 * @param vhosts
 * @return Le 1er VirtualHost qui match le port et server_name, si il n'y a aucun match, retourne le 1er VirtualHost qui match le port
 */
VirtualHost& VirtualHost::getServerByName(const std::string& name, int port, std::list<VirtualHost>& vhosts)
{
	std::list<VirtualHost*> matches;

	for (std::list<VirtualHost>::iterator vhost_it = vhosts.begin(); vhost_it != vhosts.end(); vhost_it++)
		if (ft::contains(vhost_it->getPorts(), port))
			matches.push_back(&(*vhost_it));

	if (matches.empty())
		throw std::invalid_argument("No matching server found"); // Si ça arrive c'est parce qu'on a un socket ouvert sur un port relié à aucun serveur

	for (std::list<VirtualHost*>::iterator matches_it = matches.begin(); matches_it != matches.end(); matches_it++)
		if (ft::contains((*matches_it)->getConfig().getNames(), name))
			return *(*matches_it);
	return (*matches.front());
}
