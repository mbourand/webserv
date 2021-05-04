/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/15 21:29:28 by nforay            #+#    #+#             */
/*   Updated: 2021/05/04 18:56:37 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "Logger.hpp"
#include "Utils.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Socket::Socket() : m_sockfd(-1)
{
	ft::bzero(&this->m_addr_in, sizeof(this->m_addr_in));
}

Socket::Socket(const Socket &src) : m_ipaddr(src.m_ipaddr), m_sockfd(src.m_sockfd), m_addr_in(src.m_addr_in)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Socket::~Socket()
{
	if (this->Success())
		close(this->m_sockfd);
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Socket &				Socket::operator=(Socket const &rhs)
{
	if (this != &rhs)
	{
		this->m_ipaddr = rhs.m_ipaddr;
		this->m_sockfd = rhs.m_sockfd;
		this->m_addr_in = rhs.m_addr_in;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

bool					Socket::Create(void)
{
	m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (!this->Success())
		return (false);
	int optval = 1;
	if (setsockopt(this->m_sockfd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&optval), sizeof(optval)))
		return (false);
	return (fcntl(this->m_sockfd, F_SETFL, O_NONBLOCK) != -1);
}

static std::string		my_inet_ntoa(struct in_addr in)
{
	std::stringstream ss;
	unsigned char *bytes = reinterpret_cast<unsigned char *>(&in);
	ss << static_cast<int>(bytes[0]) << "." << static_cast<int>(bytes[1]) << "." << static_cast<int>(bytes[2]) << "." << static_cast<int>(bytes[3]);
	return (ss.str());
}

static int				my_inet_aton (const char *cp, struct in_addr *inp)
{
	std::string			host(cp);
	std::string			fragment;
	in_addr_t			result;
	u_int				parts[4];
	int					i = 0;

	if (host == "localhost")
		host = "127.0.0.1";
	std::stringstream	ss(host);
	while (getline(ss, fragment, '.') && (i < 4))
		parts[i++] = ft::toInt(fragment);
	if (i != 4)
		return (0);
	if ((parts[0] > 0xff) || (parts[1] > 0xff) || (parts[2] > 0xff) || (parts[3] > 0xff))
		return (0);
	result = (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8) | parts[3];
	if (inp)
		inp->s_addr = htonl(result);
	return (1);
}

bool					Socket::Bind(int const port)
{
	if (!this->Success())
		return (false);
	this->m_addr_in.sin_family = AF_INET;
	this->m_addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
	this->m_addr_in.sin_port = htons(port);
	if (bind(this->m_sockfd, reinterpret_cast<sockaddr*>(&this->m_addr_in), sizeof(this->m_addr_in)) != 0)
		return (false);
	this->m_ipaddr = my_inet_ntoa(this->m_addr_in.sin_addr);
	return (true);
}

bool					Socket::Connect(std::string const &host, int const port)
{
	if (!this->Success())
		return (false);
	this->m_addr_in.sin_family = AF_INET;
	if (!my_inet_aton(host.c_str(), &this->m_addr_in.sin_addr))
		return (false);
	this->m_addr_in.sin_port = htons(port);
	if (connect(this->m_sockfd, reinterpret_cast<sockaddr*>(&this->m_addr_in), sizeof(this->m_addr_in)) != 0)
		return (false);
	return (true);
}

bool					Socket::Listen(void) const
{
	if (!this->Success())
		return (false);
	if (listen(this->m_sockfd, MAX_CONNECTIONS) == -1)
		return (false);
	return (true);
}

bool					Socket::Accept(Socket &connection)
{
	int	addr_length = sizeof(this->m_addr_in);
	connection.m_sockfd = accept(this->m_sockfd, reinterpret_cast<sockaddr *>(&this->m_addr_in), reinterpret_cast<socklen_t*>(&addr_length));
	if (connection.m_sockfd <= 0)
		return (false);
	connection.m_ipaddr = my_inet_ntoa(this->m_addr_in.sin_addr);
	return (true);
}

bool					Socket::Send(const std::string &msg) const
{
	if (send(this->m_sockfd, msg.c_str(), msg.size(), MSG_NOSIGNAL) == -1)
		return (false);
	return (true);
}

int						Socket::Recieve(std::string &str) const
{
	char	buff[MAX_RECIEVE + 1];

	str = "";
	ft::bzero(buff, MAX_RECIEVE + 1);
	int ret = recv(this->m_sockfd, buff, MAX_RECIEVE, 0);
	switch (ret)
	{
	case -1:
		return (Logger::print("Error in Socket::Recieve", 0, ERROR, SILENT));
	case 0:
		return (0);
	default:
		str = buff;
		return (ret);
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

bool					Socket::Success(void) const
{
	if (this->m_sockfd != -1)
		return (true);
	return (false);
}

int						Socket::GetSocket(void) const
{
	return (this->m_sockfd);
}

/* ************************************************************************** */
