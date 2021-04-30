/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/15 21:29:28 by nforay            #+#    #+#             */
/*   Updated: 2021/04/30 23:00:29 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <string>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>
# include <unistd.h>
# include <fcntl.h>
# include <arpa/inet.h>
# include "Utils.hpp"

const int MAX_CONNECTIONS = 10;
const int MAX_RECIEVE = 65534;

class Socket
{

	public:

		Socket();
		Socket(Socket const &src);
		virtual ~Socket();

		Socket &		operator=(Socket const &rhs);

		bool	Create(void);
		bool	Bind(int const port);
		bool	Connect(std::string const &host, int const port);

		bool	Listen(void) const;
		bool	Accept(Socket &connection);
		bool	Send(const std::string &msg) const;
		int		Recieve(std::string &str) const;
		bool	Success(void) const;
		int		GetSocket(void) const;

	protected:

		std::string	m_ipaddr;

	private:

		int			m_sockfd;
		sockaddr_in	m_addr_in;

};

#endif /* ********************************************************** SOCKET_H */