/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/23 19:27:25 by nforay            #+#    #+#             */
/*   Updated: 2021/04/27 19:45:15 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

# include <iostream>
# include <string>
# include "Socket.hpp"

class ClientSocket : public Socket
{

	public:

		ClientSocket(const std::string &host, int port);
		ClientSocket(ClientSocket const &src);
		virtual ~ClientSocket();

		ClientSocket &			operator=(ClientSocket const &rhs);
		const ClientSocket &	operator<< (const std::string &str) const;
		const ClientSocket &	operator>> (std::string &str) const;

		class ClientSocketException : public std::exception
		{
			public:
				ClientSocketException(std::string str) : m_message(str) {}
				virtual ~ClientSocketException() throw() {return ;}
				virtual const char* what() const throw() {return m_message.c_str();}
			
			private:
				std::string const	m_message;
		};

	private:

		ClientSocket();

};

#endif /* **************************************************** CLIENTSOCKET_H */