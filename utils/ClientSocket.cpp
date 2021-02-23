/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/23 19:27:25 by nforay            #+#    #+#             */
/*   Updated: 2021/02/23 19:37:36 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientSocket.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ClientSocket::ClientSocket()
{
}

ClientSocket::ClientSocket(std::string host, int port)
{
	if (!Socket::Create())
    {
		throw ClientSocketException("Could not create client socket.");
    }
	if (!Socket::Connect(host, port))
    {
		throw ClientSocketException("Could not bind to port.");
    }
}

ClientSocket::ClientSocket(const ClientSocket &src)
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ClientSocket::~ClientSocket()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ClientSocket &				ClientSocket::operator=( ClientSocket const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

const ClientSocket &		ClientSocket::operator<< (const std::string &str) const
{
	if (!Socket::Send(str))
    {
		throw ClientSocketException("Couldn't write to socket.");
    }
	return *this;
}

const ClientSocket &		ClientSocket::operator>> (std::string &str) const
{
	if ( ! Socket::Recieve(str))
    {
		throw ClientSocketException("Could not read from socket.");
    }
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */