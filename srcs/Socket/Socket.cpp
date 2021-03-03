/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/15 21:29:28 by nforay            #+#    #+#             */
/*   Updated: 2021/03/03 19:33:27 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "Logger.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Socket::Socket() : m_sockfd(-1)
{
	bzero(&this->m_addr_in, sizeof(this->m_addr_in));
}

Socket::Socket(const Socket &src) : m_sockfd(src.m_sockfd), m_addr_in(src.m_addr_in)
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
	(void)rhs;
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

bool					Socket::Create(void)
{
	m_sockfd = socket(AF_INET, SOCK_STREAM, 0);//Address Family : internet, Stream Socket, 0 == use TCP
	if (!this->Success())
		return (false);
	int optval = 1;
	//ne pas attendre la fin d'une précédente connexion https://bousk.developpez.com/cours/reseau-c++/TCP/08-premier-serveur-mini-serveur/
	if (setsockopt(this->m_sockfd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&optval), sizeof(optval)))
		return (false);
	return (fcntl(this->m_sockfd, F_SETFL, O_NONBLOCK) != -1);
}

bool					Socket::Bind(int const port)
{
	if (!this->Success())
		return (false);
	this->m_addr_in.sin_family = AF_INET;//https://youtu.be/bdIiTxtMaKA?t=207
	this->m_addr_in.sin_addr.s_addr = INADDR_ANY;
	this->m_addr_in.sin_port = (port >> 8) | (port << 8);//byte swap 1234 -> 4321
	if (bind(this->m_sockfd, reinterpret_cast<sockaddr*>(&this->m_addr_in), sizeof(this->m_addr_in)) != 0)
		return (false);
	return (true);
}

bool					Socket::Connect(std::string const &host, int const port)
{
	(void)host;
	if (!this->Success())
		return (false);
	this->m_addr_in.sin_family = AF_INET;//https://youtu.be/bdIiTxtMaKA?t=207
	this->m_addr_in.sin_addr.s_addr = INADDR_ANY;
	this->m_addr_in.sin_port = (port>>8) | (port<<8);//byte swap 1234 -> 4321
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
	return (true);
}

bool					Socket::Send(std::string const msg) const
{
	if (send(this->m_sockfd, msg.c_str(), msg.size(), MSG_NOSIGNAL) == -1)
		return (false);
	return (true);
}

int						Socket::Recieve(std::string &str) const
{
	char	buff[MAX_RECIEVE + 1];

	str = "";
	bzero(buff, MAX_RECIEVE + 1);
	int ret = recv(this->m_sockfd, buff, MAX_RECIEVE, 0);
	if (ret == -1)
		return Logger::print("Error in Socket::Recieve", 0, ERROR, NORMAL);
	else if (ret == 0)
		return (0);
	else
	{
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
