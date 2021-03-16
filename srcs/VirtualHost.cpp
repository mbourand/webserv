#include "VirtualHost.hpp"
#include <sstream>

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
	for (std::list<int>::const_iterator it = _config.getPorts().begin(); it != _config.getPorts().end(); it++)
		_sockets.push_back(ServerSocket(*it));
}

VirtualHost::~VirtualHost()
{}

/*
** ------------------------------- OPERATOR ------------------------------
*/

VirtualHost& VirtualHost::operator=(const VirtualHost& other)
{
	_config = other._config;
	_sockets = other._sockets;
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

const std::list<ServerSocket>& VirtualHost::getSockets() const
{
	return _sockets;
}

/*
** ------------------------------- STATIC ------------------------------
*/

VirtualHost& VirtualHost::getServerByName(const std::string& name, int port, std::list<VirtualHost>& vhosts)
{
	for (std::list<VirtualHost>::iterator vhost_it = vhosts.begin(); vhost_it != vhosts.end(); vhost_it++)
	{
		for (std::list<std::string>::const_iterator name_it = vhost_it->getConfig().getNames().begin(); name_it != vhost_it->getConfig().getNames().end(); name_it++)
		{
			if (*name_it == name)
			{
				for (std::list<int>::const_iterator port_it = vhost_it->getConfig().getPorts().begin(); port_it != vhost_it->getConfig().getPorts().end(); port_it++)
					if (*port_it == port)
						return *vhost_it;
			}
		}
	}
	throw std::invalid_argument("No server_name matches uri");
}
