/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcp-server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 01:13:41 by nforay            #+#    #+#             */
/*   Updated: 2021/03/02 03:57:30 by nforay           ###   ########.fr       */
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
		fd_set	read_sockets, read_sockets_z, write_sockets, write_sockets_z, error_sockets, error_sockets_z;
		timeval timeout = { 20000 };
		FD_ZERO(&read_sockets_z);
		FD_ZERO(&write_sockets_z);
		FD_ZERO(&error_sockets_z);
		FD_SET(server.GetSocket(), &read_sockets_z);
		FD_SET(server.GetSocket(), &write_sockets_z);
		FD_SET(server.GetSocket(), &error_sockets_z);
		std::vector<ServerSocket*> clients;
		while (42)
		{
			int highestFd = server.GetSocket();
			std::vector<ServerSocket*>::iterator			it = clients.begin();
			std::vector<ServerSocket*>::iterator			end = clients.end();
			for (; it != end; ++it)
			{
				FD_SET((*it)->GetSocket(), &read_sockets_z);
				FD_SET((*it)->GetSocket(), &write_sockets_z);
				FD_SET((*it)->GetSocket(), &error_sockets_z);
				if ((*it)->GetSocket() > highestFd)
					highestFd = (*it)->GetSocket();
			}
			read_sockets = read_sockets_z;
			write_sockets = write_sockets_z;
			error_sockets = error_sockets_z;
			if (select(highestFd + 1, &read_sockets, &write_sockets, &error_sockets, &timeout) < 0)
			{
				Logger::print("select() returned -1", NULL, ERROR, NORMAL);
				continue; //? Subject: "Your server should never die."
			}
			else
			{
				if (FD_ISSET(server.GetSocket(), &read_sockets))
				{
					ServerSocket *new_connection = new ServerSocket;
					server.Accept(*new_connection);
					clients.push_back(new_connection);
				}
				else
				{
					it = clients.begin();
					end = clients.end();
					if (clients.size() > 0)
					{
						while (it != end)
						{
							bool	error = false;
							if (FD_ISSET((*it)->GetSocket(), &error_sockets))
							{
								error = true;
								Logger::print("FD Error flagged by Select", NULL, WARNING, NORMAL);
							}
							else if (FD_ISSET((*it)->GetSocket(), &read_sockets))
							{
								try
								{
									Request req;
									std::string	data;
									*(*it) >> data;
									req.append(data);
									std::cout << "\e[33mClient Request:\nStart>|\n" << data << "\n|<End\e[39m" << std::endl;
								}
								catch(ServerSocket::ServerSocketException &e)
								{
									Logger::print(e.what(), NULL, ERROR, NORMAL);
									error = true;
								}
								if (FD_ISSET((*it)->GetSocket(), &write_sockets))
								{
									try
									{
										std::string	data;
										data = "HTTP/1.1 200 OK\r\n\r\nWelcome to Webserv!";
										*(*it) << data;
										Logger::print("Success", NULL, SUCCESS, NORMAL);
									}
									catch(ServerSocket::ServerSocketException &e)
									{
										Logger::print(e.what(), NULL, ERROR, NORMAL);
										error = true;
									}
								}
							}
							if (error)
							{
								Logger::print("Client Disconnected", NULL, WARNING, NORMAL);
								delete *it;
								it = clients.erase(it);
							}
							else
							{
								++it;
							}
						}
					}
				}
			}
		}
	}
	catch(ServerSocket::ServerSocketException& e)
	{
		Logger::print(e.what(), NULL, ERROR, NORMAL);
	}
	return (0);
}
