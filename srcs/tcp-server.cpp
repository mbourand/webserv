/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcp-server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 01:13:41 by nforay            #+#    #+#             */
/*   Updated: 2021/03/03 19:53:57 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerSocket.hpp"
#include "Logger.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <list>
#include <string.h>
#include <errno.h>

struct Client {
	ServerSocket	*sckt;
	Request			*req;
};

std::string string_to_hex(const std::string& input)
{
    static const char hex_digits[] = "0123456789ABCDEF";

    std::string output;
    output.reserve(input.length() * 2);
	for(std::string::size_type i = 0; i < input.size(); ++i)
    {
        output.push_back(hex_digits[input[i] >> 4]);
        output.push_back(hex_digits[input[i] & 15]);
		output.push_back(' ');
    }
    return output;
}

std::string string_strip_undisplayable(const std::string& input)
{
    std::string output;
    output.reserve(input.length());
	for(std::string::size_type i = 0; i < input.size(); ++i)
    {
        if (std::isprint(input[i]))
			output.push_back(input[i]);
		else
			output.push_back(' ');
    }
    return output;
}

int	main(void)
{
	Logger::setMode(NORMAL);
	Logger::print("Webserv is starting...", NULL, INFO, NORMAL);
	try
	{
		bool selecterror = true;
		ServerSocket server(8080);
		Logger::print("Webserv is ready, waiting for connection...", NULL, SUCCESS, NORMAL);
		fd_set	read_sockets, read_sockets_z, write_sockets, write_sockets_z, error_sockets, error_sockets_z;
		FD_ZERO(&read_sockets_z);
		FD_ZERO(&write_sockets_z);
		FD_ZERO(&error_sockets_z);
		FD_SET(server.GetSocket(), &read_sockets_z);
		FD_SET(server.GetSocket(), &write_sockets_z);
		FD_SET(server.GetSocket(), &error_sockets_z);
		std::list<Client> clients;
		while (42)
		{
			int highestFd = server.GetSocket();
			std::list<Client>::iterator			it = clients.begin();
			std::list<Client>::iterator			end = clients.end();
			for (; it != end; ++it)
			{
				FD_SET((*it).sckt->GetSocket(), &read_sockets_z);
				FD_SET((*it).sckt->GetSocket(), &write_sockets_z);
				FD_SET((*it).sckt->GetSocket(), &error_sockets_z);
				if ((*it).sckt->GetSocket() > highestFd)
					highestFd = (*it).sckt->GetSocket();
			}
			read_sockets = read_sockets_z;
			write_sockets = write_sockets_z;
			error_sockets = error_sockets_z;
			if (select(highestFd + 1, &read_sockets, &write_sockets, &error_sockets, NULL) < 0)
			{
				Logger::print("select() returned -1 : "+std::string(strerror(errno)), NULL, ERROR, NORMAL);
				continue; //? Subject: "Your server should never die."
			}
			else
			{
				if (FD_ISSET(server.GetSocket(), &read_sockets))
				{
					Client	new_client;
					new_client.sckt = new ServerSocket;
					new_client.req = new Request;
					server.Accept(*new_client.sckt);
					clients.push_back(new_client);
					Logger::print("Client Connected", NULL, SUCCESS, NORMAL);
				}
				else
				{
					it = clients.begin();
					end = clients.end();
					while (!clients.empty() && it != end)
					{
						bool	error = false;
						if (FD_ISSET((*it).sckt->GetSocket(), &error_sockets))
						{
							error = true;
							Logger::print("FD Error flagged by Select", NULL, WARNING, NORMAL);
						}
						else if (!error && FD_ISSET((*it).sckt->GetSocket(), &read_sockets))
						{
							std::string	data;
							try
							{
								*(*it).sckt >> data;
								(*it).req->append(data);
								std::cout << "\e[35m" << std::setfill(' ') << std::setw(MAX_RECIEVE * 2 + MAX_RECIEVE) << string_to_hex(data) << "\e[33m\t" << string_strip_undisplayable(data) << "\e[39m" << std::endl;
							}
							catch(ServerSocket::ServerSocketException &e)
							{
								Logger::print(e.what(), NULL, ERROR, NORMAL);
								error = true;
							}
							catch(std::invalid_argument &e)
							{
								Response response(400, "Bad Request");
								*(*it).sckt << response.getResponseText();
								Logger::print(e.what(), NULL, ERROR, NORMAL);
							}
							if (!error && (*it).req->isfinished() && FD_ISSET((*it).sckt->GetSocket(), &write_sockets))
							{
								try
								{
									Response response = (*it).req->_method->process(*(*it).req);
									*(*it).sckt << response.getResponseText();
									std::cout << *(*it).req << std::endl;
									std::cout << response.getResponseText() << std::endl;
									//std::string	header;
									//std::string body = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<title>Welcome to webserv!</title>\r\n<style>\r\n    body {\r\n        width: 35em;\r\n        margin: 0 auto;\r\n        font-family: Tahoma, Verdana, Arial, sans-serif;\r\n    }\r\n</style>\r\n</head>\r\n<body>\r\n<h1>Welcome to webserv!</h1>\r\n<p>If you see this page, the webserv web server is successfully installed and\r\nworking. Further configuration is required.</p>\r\n<p><em>Thank you for using webserv.</em></p>\r\n</body>\r\n</html>";
									//header = "HTTP/1.1 200 OK\r\nServer: webserv/1.0.0 (Ubuntu)\r\nDate: Wed, 03 Mar 2021 17:55:28 GMT\r\nContent-Type: text/html\r\nContent-Length: 449\r\nLast-Modified: Mon, 21 Dec 2020 14:37:37 GMT\r\nConnection: keep-alive\r\n\r\n";
									//*(*it).sckt << header;
									//*(*it).sckt << body;
									Logger::print("Success", NULL, SUCCESS, NORMAL);
								}
								catch(ServerSocket::ServerSocketException &e)
								{
									Logger::print(e.what(), NULL, ERROR, NORMAL);
								}
								delete (*it).req;
								(*it).req = new Request;
							}
						}
						if (error)
						{
							Logger::print("Client Disconnected", NULL, SUCCESS, NORMAL);
							FD_CLR((*it).sckt->GetSocket(), &read_sockets_z);
							FD_CLR((*it).sckt->GetSocket(), &write_sockets_z);
							FD_CLR((*it).sckt->GetSocket(), &error_sockets_z);
							delete (*it).sckt;
							delete (*it).req;
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
	catch(ServerSocket::ServerSocketException& e)
	{
		Logger::print(e.what(), NULL, ERROR, NORMAL);
	}
	return (0);
}
