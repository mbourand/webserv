/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcp-server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 01:13:41 by nforay            #+#    #+#             */
/*   Updated: 2021/03/18 18:03:01 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "Response.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <list>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include "Types_parser.hpp"
#include "VirtualHost.hpp"
#include "Config.h"

#ifndef DEBUG
# define DEBUG 0
#endif

t_webserv	g_webserv;

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

void	handle_new_connection(ServerSocket &server, std::list<Client> &clients)
{
	Client	new_client;
	new_client.sckt = new ServerSocket;
	new_client.req = new Request;
	new_client.sckt->setServerPort(server.getServerPort());
	server.Accept(*new_client.sckt);
	clients.push_back(new_client);
	Logger::print("New Client Connected", NULL, SUCCESS, NORMAL);
}

bool	handle_client_request(Client &client)
{
	std::string	data;
	try
	{
		*client.sckt >> data;
		client.req->append(data);
		if (DEBUG)
			std::cout << "\e[35m" << std::setfill(' ') << std::setw(MAX_RECIEVE * 2 + MAX_RECIEVE) << string_to_hex(data) << "\e[33m\t" << string_strip_undisplayable(data) << "\e[39m" << std::endl;
	}
	catch(ServerSocket::ServerSocketException &e)
	{
		Logger::print(e.what(), NULL, ERROR, NORMAL);
		return true;
	}
	catch(std::invalid_argument &e)
	{
		try
		{
			Response response(400);
			if (client.req->_error_code)
				response.setCode(client.req->_error_code);
			ConfigContext bad_request_context;
			*client.sckt << response.getResponseText(bad_request_context);
		}
		catch (std::exception& e)
		{
			Logger::print(e.what(), NULL, ERROR, NORMAL);
		}
		return true;
	}
	return false;
}

bool	handle_server_response(Client &client, std::list<VirtualHost>& vhosts)
{
	try
	{
		std::string host;
		for (Request::HeadersVector::iterator header_it = client.req->_headers.begin(); header_it != client.req->_headers.end(); header_it++)
		{
			if ((*header_it)->getType() == "Host")
			{
				host = (*header_it)->getValue();
				break;
			}
		}
		VirtualHost vhost = VirtualHost::getServerByName(host, client.sckt->getServerPort(), vhosts);
		Response response = client.req->_method->process(*client.req, vhost.getConfig());
		*client.sckt << response.getResponseText(vhost.getConfig());
		if (DEBUG)
			std::cout << *client.req << std::endl;
		if (response.getCode() != 200)
			return true;
		Logger::print("Success", NULL, SUCCESS, NORMAL);
	}
	catch(ServerSocket::ServerSocketException &e)
	{
		Logger::print(e.what(), NULL, ERROR, NORMAL);
	}
	delete client.req;
	client.req = new Request;
	return false;
}

int	main(int argc, char **argv)
{
	Logger::setMode(SILENT);
	Logger::print("Webserv is starting...", NULL, INFO, SILENT);
	if (argc > 2)
	{
		std::cerr << "Format: ./webserv <path_to_config_file>" << std::endl;
		return 1;
	}

	std::list<VirtualHost> vhosts;
	std::string config_path = (argc == 1 ? "./config/default.conf" : argv[1]);

	g_webserv.run = true;
	g_webserv.file_formatname = new HashTable(256);
	parse_types_file(g_webserv.file_formatname, "/etc/mime.types");
	sighandler();
	try
	{
		init_config(config_path, vhosts);
		fd_set							read_sockets, read_sockets_z, write_sockets, write_sockets_z, error_sockets, error_sockets_z;
		std::list<Client>				clients;
		std::list<Client>::iterator		it;
		int								highestFd = 0;

		Logger::print("Webserv is ready, waiting for connection...", NULL, SUCCESS, SILENT);
		FD_ZERO(&read_sockets_z);
		FD_ZERO(&write_sockets_z);
		FD_ZERO(&error_sockets_z);
		for (std::map<int, ServerSocket*>::iterator its = g_webserv.sockets.begin(); its != g_webserv.sockets.end(); its++)
		{
			if (its->second->GetSocket() > highestFd)
				highestFd = its->second->GetSocket();
			FD_SET(its->second->GetSocket(), &read_sockets_z);
			FD_SET(its->second->GetSocket(), &write_sockets_z);
			FD_SET(its->second->GetSocket(), &error_sockets_z);
		}
		while (g_webserv.run)
		{
			it = clients.begin();
			for (; it != clients.end(); ++it)
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
				if (g_webserv.run)
					Logger::print("Error select(): "+std::string(strerror(errno)), NULL, ERROR, NORMAL);
				continue;
			}
			else
			{
				for (std::map<int, ServerSocket*>::iterator its = g_webserv.sockets.begin(); its != g_webserv.sockets.end(); its++)
				{
					if (FD_ISSET(its->second->GetSocket(), &read_sockets))
						handle_new_connection(*its->second, clients);
				}
				it = clients.begin();
				while (it != clients.end())
				{
					bool	error = false;
					if (FD_ISSET((*it).sckt->GetSocket(), &error_sockets))
					{
						error = true;
						Logger::print("FD Error flagged by Select", NULL, WARNING, NORMAL);
					}
					else if (!error && FD_ISSET((*it).sckt->GetSocket(), &read_sockets))
					{
						error = handle_client_request((*it));
						if (!error && (*it).req->isfinished() && FD_ISSET((*it).sckt->GetSocket(), &write_sockets))
							error = handle_server_response((*it), vhosts);
					}
					if (error)
					{
						Logger::print("Client Disconnected", NULL, SUCCESS, VERBOSE);
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
		Logger::print("Webserv is shutting down...", NULL, INFO, SILENT);
		it = clients.begin();
		while (it != clients.end())
		{
			Logger::print("Socket closed", NULL, SUCCESS, SILENT);
			delete (*it).sckt;
			delete (*it).req;
			it = clients.erase(it);
		}
	}
	catch(ServerSocket::ServerSocketException& e)
	{
		Logger::print(e.what(), NULL, ERROR, NORMAL);
	}
	for (std::map<int, ServerSocket*>::iterator its = g_webserv.sockets.begin(); its != g_webserv.sockets.end(); its++)
	{
		std::cout << "DELETEING....." << std::endl;
		delete its->second;
	}
	delete g_webserv.file_formatname;
	Logger::print("Webserv Shutdown complete", NULL, SUCCESS, SILENT);
	std::cout << std::flush;
	return (0);
}
