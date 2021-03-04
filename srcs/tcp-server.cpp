/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcp-server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbourand <mbourand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 01:13:41 by nforay            #+#    #+#             */
/*   Updated: 2021/03/04 16:37:52 by mbourand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerSocket.hpp"
#include "Logger.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Config.hpp"
#include <string>
#include <iostream>
#include <fstream>

int	main(int argc, char **argv)
{
	std::string config_path = "config/default.conf";
	if (argc >= 2)
		config_path = argv[1];
	std::ifstream config_file(config_path.c_str(), std::ifstream::in);
	std::string config_content((std::istreambuf_iterator<char>(config_file)), (std::istreambuf_iterator<char>()));

	Config config(config_content);

	std::cout << config.getServers().size() << std::endl;
	for (std::list<ServerConfig>::iterator it = config.getServers().begin(); it != config.getServers().end(); it++)
		std::cout << it->getLocations().size() << " " << it->getParams().size() << std::endl;

	Logger::setMode(NORMAL);
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
				Logger::print("Client Request:\nStart>|\n" + data + "\n|<End", NULL, INFO, VERBOSE);

				Response response = req._method->process(req);
				new_connection << response.getResponseText();
				Logger::print("Response sent successfully", NULL, SUCCESS, NORMAL);
			}
			catch(ServerSocket::ServerSocketException &e)
			{
				Logger::print(e.what(), NULL, ERROR, NORMAL);
			}
			catch(std::invalid_argument& e)
			{
				Response response(400, "Bad Request");
				new_connection << response.getResponseText();
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
