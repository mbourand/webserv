#ifndef SERVER_HPP
#define SERVER_HPP

#include "ServerSocket.hpp"
#include "ServerConfig.hpp"

class VirtualHost
{
	private:
		ServerConfig _config;
		std::list<ServerSocket> _sockets;

	public:
		VirtualHost();
		VirtualHost(const VirtualHost& other);
		VirtualHost(const ServerConfig& config);
		virtual ~VirtualHost();
		VirtualHost& operator=(const VirtualHost& other);

		const ServerConfig& getConfig() const;
		const std::list<ServerSocket>& getSockets() const;

		static VirtualHost& getServerByName(const std::string& name, int port, std::list<VirtualHost>& vhosts);
};

#endif
