/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcp-server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 01:13:41 by nforay            #+#    #+#             */
/*   Updated: 2021/02/16 02:38:09 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerSocket.hpp"
#include <string>
#include <iostream>

int	main(void)
{
	std::cout << "Webserv is starting..." << std::endl;
	try
	{
		ServerSocket server(18000);
		while (42)
		{
			ServerSocket new_connection;
			std::cout << "Webserv is ready, waiting for connection..." << std::endl;
			server.Accept(new_connection);
			try
			{
				while (42)
				{
					std::string	data;
					new_connection >> data;
					std::cout << data << std::endl;
					data = "HTTP/1.1 200 OK\r\n\r\nWelcome to Webserv!";
					new_connection << data;
				}
			}
			catch(ServerSocket::ServerSocketException&)
			{}
		}
	}
	catch(ServerSocket::ServerSocketException& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (0);
}
