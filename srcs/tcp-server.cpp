/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcp-server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 01:13:41 by nforay            #+#    #+#             */
/*   Updated: 2021/04/09 18:10:25 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "Response.hpp"
#include <string>
#include <iostream>
#include <iomanip>
#include <list>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include "Types_parser.hpp"
#include "VirtualHost.hpp"
#include "Threadpool.hpp"
#include "Config.h"
#include "Utils.hpp"

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

void	hexdump_str(const std::string &data, int size)
{
	for (size_t i = 0; i < data.size(); i += size)
	{
		std::string	chunk = data.substr(i, size);
		std::cout << "\e[35m" << std::setfill(' ') << std::setw(size * 3) << string_to_hex(chunk) << "\e[33m\t" << string_strip_undisplayable(chunk) << "\e[39m" << std::endl;
	}
}

bool	handle_client_request(Client &client)
{
	std::string	data;
	try
	{
		*client.sckt >> data;
		client.req->append(data);
		if (DEBUG)
			hexdump_str(data, 32);
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
			Response response(400, "");
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

bool	handle_server_response(Client &client)
{
	try
	{
		std::string host;
		for (Request::HeadersVector::iterator header_it = client.req->_headers.begin(); header_it != client.req->_headers.end(); header_it++)
		{
			if ((*header_it)->getType() == "Host")
			{
				host = (*header_it)->getValue().substr(0, std::min((*header_it)->getValue().find(':'), (*header_it)->getValue().size()));
				break;
			}
		}
		if (DEBUG)
			std::cout << *client.req << std::endl;
		VirtualHost vhost = VirtualHost::getServerByName(host, client.sckt->getServerPort(), g_webserv.vhosts);
		Response response = client.req->_method->process(*client.req, vhost.getConfig(), *client.sckt);
		*client.sckt << response.getResponseText(vhost.getConfig());
		if (response.getCode() != 200)
			return true;
		Logger::print("Success", NULL, SUCCESS, NORMAL);
	}
	catch(ServerSocket::ServerSocketException &e)
	{
		Logger::print(e.what(), NULL, ERROR, NORMAL);
	}
	if (client.req && client.sckt->Success())
	{
		delete client.req;
		client.req = new Request;
	}
	return false;
}

void init_factories()
{
	g_webserv.methods.add(new ConnectMethod());
	g_webserv.methods.add(new DeleteMethod());
	g_webserv.methods.add(new GetMethod());
	g_webserv.methods.add(new HeadMethod());
	g_webserv.methods.add(new OptionsMethod());
	g_webserv.methods.add(new PostMethod());
	g_webserv.methods.add(new PutMethod());
	g_webserv.methods.add(new TraceMethod());

	g_webserv.headers.add(new AcceptCharsetsHeader());
	g_webserv.headers.add(new AcceptLanguageHeader());
	g_webserv.headers.add(new AllowHeader());
	g_webserv.headers.add(new AuthorizationHeader());
	g_webserv.headers.add(new ContentLanguageHeader());
	g_webserv.headers.add(new ContentLengthHeader());
	g_webserv.headers.add(new ContentLocationHeader());
	g_webserv.headers.add(new ContentTypeHeader());
	g_webserv.headers.add(new DateHeader());
	g_webserv.headers.add(new HostHeader());
	g_webserv.headers.add(new LastModifiedHeader());
	g_webserv.headers.add(new LocationHeader());
	g_webserv.headers.add(new RefererHeader());
	g_webserv.headers.add(new RetryAfterHeader());
	g_webserv.headers.add(new ServerHeader());
	g_webserv.headers.add(new TransferEncodingHeader());
	g_webserv.headers.add(new UserAgentHeader());
	g_webserv.headers.add(new WWWAuthenticateHeader());
}

int	main(int argc, char **argv)
{
	Logger::setMode(NORMAL);
	Logger::print("Webserv is starting...", NULL, INFO, SILENT);
	if (argc > 2)
	{
		std::cerr << "Format: ./webserv <path_to_config_file>" << std::endl;
		return 1;
	}

	std::string config_path = (argc == 1 ? "./config/default.conf" : argv[1]);

	g_webserv.run = true;
	g_webserv.file_formatname = new HashTable(256);
	g_webserv.cwd = ft::get_cwd();
	init_factories();
	Threadpool workers(0);//positive number to enable, todo: get number of workers from config
	parse_types_file(g_webserv.file_formatname, "/etc/mime.types");
	sighandler();
	try
	{
		try
		{
			init_config(config_path, g_webserv.vhosts);
		}
		catch(const std::exception& e)
		{
			return Logger::print(std::string("Invalid config file: ") + e.what(), 1, ERROR, SILENT);
		}
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
				FD_SET(it->sckt->GetSocket(), &read_sockets_z);
				FD_SET(it->sckt->GetSocket(), &write_sockets_z);
				FD_SET(it->sckt->GetSocket(), &error_sockets_z);
				if (it->sckt->GetSocket() > highestFd)
					highestFd = it->sckt->GetSocket();
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
					if (FD_ISSET(it->sckt->GetSocket(), &error_sockets))
					{
						error = true;
						Logger::print("FD Error flagged by Select", NULL, WARNING, NORMAL);
					}
					if (!error && FD_ISSET(it->sckt->GetSocket(), &read_sockets))
						error = handle_client_request((*it));
					if (!error && it->req->isfinished() && FD_ISSET(it->sckt->GetSocket(), &write_sockets))
						error = workers.AddJob((*it)); //handle_server_response((*it));
					if (error)
					{
						Logger::print("Client Disconnected", NULL, SUCCESS, VERBOSE);
						FD_CLR(it->sckt->GetSocket(), &read_sockets_z);
						FD_CLR(it->sckt->GetSocket(), &write_sockets_z);
						FD_CLR(it->sckt->GetSocket(), &error_sockets_z);
						delete it->sckt;
						delete it->req;
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
			delete it->sckt;
			delete it->req;
			it = clients.erase(it);
		}
	}
	catch(ServerSocket::ServerSocketException& e)
	{
		Logger::print(e.what(), NULL, ERROR, NORMAL);
	}

	for (std::map<int, ServerSocket*>::iterator its = g_webserv.sockets.begin(); its != g_webserv.sockets.end(); its++)
		delete its->second;
	delete g_webserv.file_formatname;
	Logger::print("Webserv Shutdown complete", NULL, SUCCESS, SILENT);
	std::cout << std::flush;
	return (0);
}
