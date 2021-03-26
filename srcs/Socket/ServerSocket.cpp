/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/15 21:34:16 by nforay            #+#    #+#             */
/*   Updated: 2021/03/25 00:00:00 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerSocket.hpp"
#include <errno.h>
#include <string.h>
#include <sstream>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ServerSocket::ServerSocket(int port) : m_port(port)
{
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
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

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

void					ServerSocket::setServerPort(int port)
{
	m_port = port;
	std::ostringstream	convert;
	convert << m_port;
	m_port_str = convert.str();
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

int 					ServerSocket::getServerPort() const
{
	return m_port;
}

std::string				ServerSocket::getServerPort_Str() const
{
	return m_port_str;
}


/* ************************************************************************** */
