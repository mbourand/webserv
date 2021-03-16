#ifndef SERVER_HPP
#define SERVER_HPP

#include "ServerSocket.hpp"
#include "ConfigContext.hpp"

class VirtualHost
{
	private:
		ConfigContext _config;
		std::list<ServerSocket> _sockets;

	public:
		VirtualHost();
		VirtualHost(const VirtualHost& other);
		VirtualHost(const ConfigContext& config);
		virtual ~VirtualHost();
		VirtualHost& operator=(const VirtualHost& other);

		const ConfigContext& getConfig() const;
		const std::list<ServerSocket>& getSockets() const;

		std::string toString() const;

		static VirtualHost& getServerByName(const std::string& name, int port, std::list<VirtualHost>& vhosts);
};

#endif
