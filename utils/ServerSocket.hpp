/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/15 21:34:16 by nforay            #+#    #+#             */
/*   Updated: 2021/02/15 21:54:16 by nforay           ###   ########.fr       */
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

		ServerSocket(int port);
		ServerSocket(ServerSocket const &src);
		virtual ~ServerSocket();

		void					Accept(ServerSocket &connection);

		ServerSocket &			operator=( ServerSocket const & rhs );
		const ServerSocket &	operator<<(const std::string &str) const;
		const ServerSocket &	operator>>(std::string &str) const;

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

/*std::ostream &			operator<<( std::ostream & o, ServerSocket const & i );*/

#endif /* **************************************************** SERVERSOCKET_H */