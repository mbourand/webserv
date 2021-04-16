/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcp-server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbourand <mbourand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 01:13:41 by nforay            #+#    #+#             */
/*   Updated: 2021/04/16 15:42:06 by mbourand         ###   ########.fr       */
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
	new_client.req = new Request(server.getServerPort());
	new_client.sckt->setServerPort(server.getServerPort());
	server.Accept(*new_client.sckt);
	if (clients.size() >= 1000) // get max_connections from config ? choose value depending of # file descriptor available
	{
		Logger::print("Webserv overloaded, New Client bounced back ("+new_client.sckt->getIPAddress()+")", NULL, ERROR, NORMAL);
		Response response(503, new_client.req->_url._path);
		response.addHeader("Connection", "close");
		response.addHeader("Retry-After", "120");
		*new_client.sckt << response.getResponseText(g_webserv.vhosts.front().getConfig());
		delete new_client.sckt;
		delete new_client.req;
		return;
	}
	clients.push_back(new_client);
	Logger::print("New Client Connected ("+new_client.sckt->getIPAddress()+")", NULL, SUCCESS, NORMAL);
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
		Logger::print(e.what(), NULL, ERROR, SILENT);
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
		if (DEBUG)
			std::cout << *client.req << std::endl;
		VirtualHost vhost = VirtualHost::getServerByName(client.req->getHeaderValue("Host"), client.sckt->getServerPort(), g_webserv.vhosts);
		Response response = client.req->_method->process(*client.req, vhost.getConfig().getConfigPath(client.req->_url._path), *client.sckt);
		*client.sckt << response.getResponseText(vhost.getConfig().getConfigPath(client.req->_url._path));
		if (response.getCode() != 200)
			return true;
		Logger::print("Success", NULL, SUCCESS, VERBOSE);
	}
	catch(ServerSocket::ServerSocketException &e)
	{
		Logger::print(e.what(), NULL, ERROR, NORMAL);
	}
	if (client.req && client.sckt->Success())
	{
		delete client.req;
		client.req = new Request(client.sckt->getServerPort());
	}
	return false;
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

	try { g_webserv.init_config(std::string(argc == 1 ? "./config/default.conf" : argv[1])); }
	catch (std::exception& e) { return Logger::print(std::string("Invalid config file: ") + e.what(), 1, ERROR, SILENT); }

	Threadpool* workers = new Threadpool(g_webserv.workers_amount);
	sighandler();

	try
	{
		fd_set							read_sockets, read_sockets_z, write_sockets, write_sockets_z, error_sockets, error_sockets_z;
		std::list<Client>				clients;
		std::list<Client>::iterator		it;
		int								highestFd = 0;
		int								serverFd = 0;
		int								readyfd = 0;
		//struct timeval 					timeout;

		Logger::print("Webserv is ready, waiting for connection...", NULL, SUCCESS, SILENT);
		FD_ZERO(&read_sockets_z);
		FD_ZERO(&write_sockets_z);
		FD_ZERO(&error_sockets_z);
		for (std::map<int, ServerSocket*>::iterator its = g_webserv.sockets.begin(); its != g_webserv.sockets.end(); its++)
		{
			if (its->second->GetSocket() > serverFd)
				serverFd = its->second->GetSocket();
			FD_SET(its->second->GetSocket(), &read_sockets_z);
		}
		while (g_webserv.run)
		{
			it = clients.begin();
			read_sockets = read_sockets_z;
			write_sockets = write_sockets_z;
			error_sockets = error_sockets_z;
			highestFd = serverFd;
			for (; it != clients.end(); ++it)
			{
				FD_SET(it->sckt->GetSocket(), &read_sockets);
				if (it->req->isfinished())
					FD_SET(it->sckt->GetSocket(), &write_sockets);
				FD_SET(it->sckt->GetSocket(), &error_sockets);
				if (it->sckt->GetSocket() > highestFd)
					highestFd = it->sckt->GetSocket();
			}
			struct timeval timeout = {30, 0};
			readyfd = select(highestFd + 1, &read_sockets, &write_sockets, &error_sockets, &timeout);
			switch (readyfd)
			{
			case -1:
				if (g_webserv.run)
					Logger::print("Error select(): "+std::string(strerror(errno)), NULL, ERROR, NORMAL);
				break;
			case 0:
				Logger::print(std::string("Idle.. Active Connections: ")+ft::toString(clients.size()), NULL, INFO, NORMAL);
				break;//webserv was idling for the past 30s
			default:
				if (!readyfd)
					break;
				for (std::map<int, ServerSocket*>::iterator its = g_webserv.sockets.begin(); its != g_webserv.sockets.end(); its++)
				{
					if (FD_ISSET(its->second->GetSocket(), &read_sockets))
					{
						handle_new_connection(*its->second, clients);
						readyfd--;
					}
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
					{
						error = handle_client_request((*it));
						readyfd--;
					}
					if (!error && FD_ISSET(it->sckt->GetSocket(), &write_sockets))
					{
						error = workers->AddJob((*it));
						readyfd--;
					}
					if (error)
					{
						bool busy = false;
						if (!workers->getJobs().empty())
							for (std::deque<Client*>::const_iterator jobs_it = workers->getJobs().begin(); jobs_it != workers->getJobs().end(); jobs_it++)
								if (it->sckt == (*jobs_it)->sckt)
									busy = true;
						if (!workers->getCurrentJobs().empty())
							for (std::list<Client*>::const_iterator jobs_it = workers->getCurrentJobs().begin(); jobs_it != workers->getCurrentJobs().end(); jobs_it++)
								if (it->sckt == (*jobs_it)->sckt)
									busy = true;
						if (busy)
							continue;

						Logger::print("Client Disconnected", NULL, SUCCESS, NORMAL);
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
				break;
			}
		}
		Logger::print("Webserv is shutting down...", NULL, INFO, SILENT);
		delete workers;
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
		g_webserv.run = false;
		Logger::print(e.what(), NULL, ERROR, SILENT);
	}

	for (std::map<int, ServerSocket*>::iterator its = g_webserv.sockets.begin(); its != g_webserv.sockets.end(); its++)
		delete its->second;
	delete g_webserv.file_formatname;
	Logger::print("Webserv Shutdown complete", NULL, SUCCESS, SILENT);
	std::cout << std::flush;
	return (0);
}
