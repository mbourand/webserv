/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbourand <mbourand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/15 21:34:16 by nforay            #+#    #+#             */
/*   Updated: 2021/03/18 14:53:16 by mbourand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerSocket.hpp"
#include <errno.h>
#include <string.h>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ServerSocket::ServerSocket(int port) : m_port(port)
{
	std::cout << "Contructeur " << port << std::endl;
	if (!Socket::Create())
		throw ServerSocketException("Counldn't create socket.");
	if (!Socket::Bind(this->m_port))
		throw ServerSocketException("Counldn't bind to port.");
	if (!Socket::Listen())
		throw ServerSocketException("Counldn't listen to socket.");
}

ServerSocket::ServerSocket(const ServerSocket &src) : m_port(src.m_port)
{
	if (!Socket::Create())
		throw ServerSocketException("Counldn't create socket.");
	if (!Socket::Bind(this->m_port))
		throw ServerSocketException("Counldn't bind to port.");
	if (!Socket::Listen())
		throw ServerSocketException("Counldn't listen to socket.");
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ServerSocket::~ServerSocket()
{
	std::cout << "Destructeur " << m_port << std::endl;
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ServerSocket &				ServerSocket::operator=( ServerSocket const & rhs )
{
	(void)rhs;
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

/*std::ostream &			operator<<( std::ostream & o, ServerSocket const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}*/

const ServerSocket &	ServerSocket::operator<<(const std::string& s) const
{
	if (!Socket::Send(s))
		throw ServerSocketException("Couldn't write to socket.");
	return *this;
}


const ServerSocket &	ServerSocket::operator>>(std::string& s) const
{
	if (!Socket::Recieve(s))
		throw ServerSocketException("Couldn't read from socket.");
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void					ServerSocket::Accept(ServerSocket &connection)
{
	if (!Socket::Accept(connection))
		throw ServerSocketException("Couldn't accept socket: "+std::string(strerror(errno)));
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

int ServerSocket::getPort() const
{
	return m_port;
}


/* ************************************************************************** */
