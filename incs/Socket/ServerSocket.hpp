/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/15 21:34:16 by nforay            #+#    #+#             */
/*   Updated: 2021/03/18 17:00:43 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERSOCKET_HPP
# define SERVERSOCKET_HPP

# include <iostream>
# include <string>
# include "Socket.hpp"

class ServerSocket : public Socket
{

	public:
		ServerSocket(void) {};
		ServerSocket(int port);
		ServerSocket(ServerSocket const &src);
		virtual ~ServerSocket();

		void					Accept(ServerSocket &connection);

		const ServerSocket &	operator<<(const std::string &str) const;
		const ServerSocket &	operator>>(std::string &str) const;
		void					setServerPort(int port);

		int						getServerPort() const;

		class ServerSocketException : public std::exception
		{
			public:
				ServerSocketException(std::string str) : m_message(str) {}
				virtual ~ServerSocketException() throw() {return ;}
				virtual const char* what() const throw() {return m_message.c_str();}

			private:
				std::string const	m_message;
		};

	private:

		int	m_port;

};

#endif /* **************************************************** SERVERSOCKET_H */
