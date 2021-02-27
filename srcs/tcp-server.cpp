/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcp-server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 01:13:41 by nforay            #+#    #+#             */
/*   Updated: 2021/02/27 18:12:55 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerSocket.hpp"
#include "Logger.hpp"
#include "Request.hpp"
#include <string>
#include <iostream>

int	main(void)
{
	Logger::setMode(VERBOSE);
	Logger::print("Webserv is starting...", NULL, INFO, NORMAL);
	try
	{
		ServerSocket server(8080);
		Logger::print("Webserv is ready, waiting for connection...", NULL, SUCCESS, NORMAL);
		while (42)
		{
			ServerSocket new_connection;
			server.Accept(new_connection);
			try
			{
				Request req;
				std::string	data;
				new_connection >> data;
				req.append(data);
				std::cout << "\e[33mClient Request:\nStart>|\n" << data << "\n|<End\e[39m" << std::endl;
				data = "HTTP/1.1 200 OK\r\n\r\nWelcome to Webserv!";
				new_connection << data;
				Logger::print("Success", NULL, SUCCESS, NORMAL);
			}
			catch(ServerSocket::ServerSocketException &e)
			{
				Logger::print(e.what(), NULL, ERROR, NORMAL);
			}
		}
	}
	catch(ServerSocket::ServerSocketException& e)
	{
		Logger::print(e.what(), NULL, ERROR, NORMAL);
	}
	return (0);
}
