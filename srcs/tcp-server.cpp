/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcp-server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbourand <mbourand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 01:13:41 by nforay            #+#    #+#             */
/*   Updated: 2021/05/28 22:23:00 by mbourand         ###   ########.fr       */
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
	if (clients.size() >= g_webserv.max_connections)
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
		if (g_webserv.debug && !client.req->_header_section_finished)
			hexdump_str(data, 24);
		client.req->append(data);
	}
	catch(ServerSocket::ServerSocketException &e)
	{
		return (Logger::print(e.what(), true, ERROR, NORMAL));
	}
	catch(std::invalid_argument &e)
	{
		// Si error_code est set on considère que ce n'est pas une erreur étant donné qu'on va pouvoir répondre avec _error_code de la request
		return (Logger::print(e.what(), (client.req->_error_code == 0), ERROR, SILENT));
	}
	return false;
}

bool	handle_server_response(Client &client)
{
	try
	{
		if (g_webserv.debug)
			std::cout << *client.req << std::endl;
		VirtualHost vhost = VirtualHost::getServerByName(client.req->getHeaderValue("Host"), client.sckt->getServerPort(), g_webserv.vhosts);
		Response response;
		if (client.req->_error_code)
			response.setCode(client.req->_error_code);
		try
		{
			if (!response.getCode() && !vhost.getConfig().getConfigPath(client.req->_url._path).getParam("auth_basic").front().empty())
			{
				std::string	credentials = client.req->getHeaderValue(AuthorizationHeader().getType());
					if (credentials.empty())
					{
						response.setCode(401); //Unauthorized (force sign-in)
						response.addHeader(WWWAuthenticateHeader().getType(), "Basic realm=\""+vhost.getConfig().getConfigPath(client.req->_url._path).getParam("auth_basic").front()+"\"");
					}
					else if (!g_webserv.creds->Check_Credentials(vhost.getConfig().getConfigPath(client.req->_url._path).getParam("auth_basic_user_file").front(), credentials.substr(6))) //TODO: comparer credentials avec ceux dans le fichier	login:password -> bG9naW46cGFzc3dvcmQ=
						response.setCode(403); //forbidden (wrong credentials)
			}
		}
		catch (std::exception& e)
		{}
		if (!response.getCode())
		{
			if (!ft::contains(vhost.getConfig().getConfigPath(client.req->_url._path).getAllowedMethods(), client.req->_method))
				response.setCode(405);
		}
		if (!response.getCode())
			response = client.req->_method->process(*client.req, vhost.getConfig().getConfigPath(client.req->_url._path), *client.sckt);
		*client.sckt << response.getResponseText(vhost.getConfig().getConfigPath(client.req->_url._path));
		if (g_webserv.debug)
			std::cout << "served response " << response.getCode() << std::endl;
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

static void		handle_terminal_input(const std::string& input, const std::list<Client> &clients, struct timeval *start, Threadpool* workers)
{
	if (input == "stop")
		g_webserv.run = false;
	else if (input == "help")
		std::cout << "\e[34m" << "Available commands are: help|stop|gzip|deflate|info|workers|uptime|logger|debug" << "\e[39m" << std::endl;
	else if (input == "debug")
	{
		g_webserv.debug = !g_webserv.debug;
		std::cout << "\e[34m" << "Debug print is now "+std::string(g_webserv.debug ? "enabled" : "disabled") << "\e[39m" << std::endl;
	}
	else if (input == "logger")
	{
		switch (Logger::getMode())
		{
		case VERBOSE:
			Logger::setMode(NORMAL);
			std::cout << "\e[34m" << "LoggerMode is now: NORMAL" << "\e[39m" << std::endl;
			break;
		case NORMAL:
			Logger::setMode(SILENT);
			std::cout << "\e[34m" << "LoggerMode is now: SILENT" << "\e[39m" << std::endl;
			break;
		default:
			Logger::setMode(VERBOSE);
			std::cout << "\e[34m" << "LoggerMode is now: VERBOSE" << "\e[39m" << std::endl;
			break;
		}
	}
	else if (input == "gzip")
	{
		g_webserv.compression_gzip = !g_webserv.compression_gzip;
		std::cout << "\e[34m" << "Plugin gzip is now "+std::string(g_webserv.compression_gzip ? "enabled" : "disabled") << "\e[39m" << std::endl;
	}
	else if (input == "deflate")
	{
		g_webserv.compression_deflate = !g_webserv.compression_deflate;
		std::cout << "\e[34m" << "Plugin deflate is now "+std::string(g_webserv.compression_deflate ? "enabled" : "disabled") << "\e[39m" << std::endl;
	}
	else if (input == "workers")
	{
		std::string	details("Workers Job Queue:\n");
		if (!workers->getJobs().empty())
		{
			workers->Lock();
			for (std::deque<Client*>::const_iterator jobs_it = workers->getJobs().begin(); jobs_it != workers->getJobs().end(); jobs_it++)
				details += "\t"+(*jobs_it)->req->_method->getType()+" "+(*jobs_it)->req->_url._path+" -> "+(*jobs_it)->sckt->getIPAddress()+"\n";
			workers->Unlock();
		}
		else
			details += "empty";
		std::cout << "\e[34m" << details << "\e[39m" << std::endl;
	}
	else if (input == "info")
	{
		std::cout << "\e[34m" << "Webserv is serving "+ft::toString(clients.size())+" clients (max: "+ft::toString(g_webserv.max_connections)+")" << "\e[39m" << std::endl;
		for (std::list<Client>::const_iterator it = clients.begin(); it != clients.end(); it++)
		{
			std::string	details("Client information:\n");
			details += "\tIP: "+it->sckt->getIPAddress()+"\n";
			details += "\tEntry Port: "+it->sckt->getServerPort_Str()+"\n";
			details += "\tSocket FD: "+ft::toString(it->sckt->GetSocket())+"\n";
			workers->Lock();
			if (!workers->getJobs().empty())
				for (std::deque<Client*>::const_iterator jobs_it = workers->getJobs().begin(); jobs_it != workers->getJobs().end(); jobs_it++)
					if (it->sckt == (*jobs_it)->sckt)
						details += "\tQueued Job: "+(*jobs_it)->req->_method->getType()+" "+(*jobs_it)->req->_url._path+"\n";
			if (!workers->getCurrentJobs().empty())
				for (std::list<Client*>::const_iterator jobs_it = workers->getCurrentJobs().begin(); jobs_it != workers->getCurrentJobs().end(); jobs_it++)
					if (it->sckt == (*jobs_it)->sckt)
						details += "\tWorker Assigned: "+(*jobs_it)->req->_method->getType()+" "+(*jobs_it)->req->_url._path+"\n";
			workers->Unlock();
			std::cout << "\e[34m" << details << "\e[39m" << std::endl;
		}
	}
	else if (input == "uptime")
	{
		struct timeval 		now;
		int					hours, mins, secs;

		hours = mins = 0;
		gettimeofday(&now, NULL);
		secs = now.tv_sec - start->tv_sec;
		while (secs >= 60)
		{
			mins++;
			secs -= 60;
		}
		while (mins >= 60)
		{
			hours++;
			mins -= 60;
		}
		std::cout << "\e[34m" << "Webserv uptime is "+ft::toString(hours)+" hour(s) "+ft::toString(mins)+" minute(s) and "+ft::toString(secs)+" second(s)." << "\e[39m" << std::endl;
	}
	else
		std::cout << "\e[34m" << "Unknown Command" << (input.empty() ? "." : ": \""+input+"\"") << "\e[39m" << std::endl;
}

int	main(int argc, char **argv)
{
	std::cout << "\e[34m" << " __      __      ___.                              \n/  \\    /  \\ ____\\_ |__   ______ ______________  __\n\\   \\/\\/   // __ \\| __ \\ /  ___// __ \\_  __ \\  \\/ /\n \\        /\\  ___/| \\_\\ \\\\___ \\\\  ___/|  | \\/\\   / \n  \\__/\\  /  \\___  >___  /____  >\\___  >__|    \\_/  \n       \\/       \\/    \\/     \\/     \\/         V1.0.0\e[39m" << std::endl;
	std::cout << "\e[34m" << "Available commands are: help|stop|gzip|deflate|info|workers|uptime|logger|debug" << "\e[39m" << std::endl;
	Logger::setMode(SILENT);
	Logger::print("Webserv is starting...", NULL, INFO, SILENT);
	if (argc > 2)
	{
		std::cerr << "Format: ./webserv <path_to_config_file>" << std::endl;
		return 1;
	}

	try { g_webserv.init_config(std::string(argc == 1 ? "./config/default.conf" : argv[1])); }
	catch (std::exception& e) { return Logger::print(std::string("Invalid config file: ") + e.what(), 1, ERROR, SILENT); }

	g_webserv.creds = new Credentials();
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
		struct timeval 					start_time;

		gettimeofday(&start_time, NULL);
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
		FD_SET(STDIN_FILENO, &read_sockets_z);
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
					Logger::print("Error select(): "+std::string(strerror(errno)), NULL, ERROR, SILENT);
				break;
			case 0:
				Logger::print(std::string("Idle.. Active Connections: ")+ft::toString(clients.size()), NULL, INFO, NORMAL);
				break;//webserv was idling for the past 30s
			default:
				if (FD_ISSET(STDIN_FILENO, &read_sockets))
				{
					std::string line;
					std::getline(std::cin, line);
					handle_terminal_input(line, clients, &start_time, workers);
					readyfd--;
				}
				for (std::map<int, ServerSocket*>::iterator its = g_webserv.sockets.begin(); its != g_webserv.sockets.end(); its++)
				{
					if (FD_ISSET(its->second->GetSocket(), &read_sockets))
					{
						handle_new_connection(*its->second, clients);
						readyfd--;
					}
				}
				it = clients.begin();
				while (readyfd && it != clients.end())
				{
					bool	error = false;
					if (FD_ISSET(it->sckt->GetSocket(), &error_sockets))
					{
						error = true;
						Logger::print("FD Error flagged by Select", NULL, WARNING, SILENT);
					}
					if (!error && FD_ISSET(it->sckt->GetSocket(), &read_sockets))
					{
						error = handle_client_request((*it));
						readyfd--;
					}
					if (!error && FD_ISSET(it->sckt->GetSocket(), &write_sockets))
					{
						if (it->req->isfinished())
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
	delete g_webserv.languages;
	delete g_webserv.creds;
	Logger::print("Webserv Shutdown complete", NULL, SUCCESS, SILENT);
	std::cout << std::flush;
	return (0);
}
