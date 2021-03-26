/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 16:34:07 by nforay            #+#    #+#             */
/*   Updated: 2021/03/27 00:37:35 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sstream>
#include <fstream>
#include <sys/time.h>
#include <time.h>
#include "CGI.hpp"
#include "Logger.hpp"
#include "ServerSocket.hpp"
#include <string.h> // !remove! use libft (strncpy)

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CGI::CGI(const Request& request, const ConfigContext& config, const ServerSocket& socket)
{
	Header				*header_found;
	std::ostringstream	convert;

	m_args.push_back("/usr/bin/php-cgi"); //pick CGI path from config // tester/ubuntu_cgi_tester
	m_args.push_back("./www/index.php"); //TODO get URI from request
	m_c_args = new char*[m_args.size() + 1];
	for (size_t i = 0; i < m_args.size(); i++)
	{
		m_c_args[i] = new char[m_args[i].size() + 1];
		strncpy(m_c_args[i], m_args[i].c_str(), m_args[i].size() + 1);
	}
	m_c_args[m_args.size()] = NULL;
	if ((header_found = request._headerFactory.getByType(WWWAuthenticateHeader().getType())) != NULL)
		m_env.push_back("AUTH_TYPE="+header_found->getValue());
	if (request._method->requestHasBody() && !request._body.empty())
	{
		convert << request._body.size();
		m_env.push_back("CONTENT_LENGTH="+convert.str());
		if ((header_found = request._headerFactory.getByType(ContentTypeHeader().getType())) != NULL)
			m_env.push_back("CONTENT_TYPE="+header_found->getValue());
		else
			m_env.push_back("CONTENT_TYPE=US-ASCII");
	}
	m_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	m_env.push_back("PATH_INFO=/"); //PATH_INFO //For example, if you execute www.placeholder.com/cgi-bin/hello.pl/index.html, then the PATH_INFO for this would be the characters that come after the CGI script name or /index.html in this example.
	m_env.push_back("PATH_TRANSLATED=www/index.php"); //PATH_TRANSLATED (use getcwd() + config root + path)
	if (!request._query_string.empty())
		m_env.push_back("QUERY_STRING="+request._query_string);
	//m_env.push_back("REMOTE_ADDR=xxx.xxx.xxx.xxx"); //REMOTE_ADDR TODO get client IP from socket
	//m_env.push_back("REMOTE_IDENT=something"); //REMOTE_IDENT This variable stores the user ID running the CGI script. The user ID is stored only if the ident process is running since ident returns a response containing not only user ID information, but also the name of the OS running the script.
	//if ((header_found = request._headerFactory.getByType(WWWAuthenticateHeader().getType())) != NULL)
	//	m_env.push_back("REMOTE_USER=something"); //REMOTE_USER Querying the REMOTE_USER variable will give the user name information of the entity making the request. This is only valid if authentication is enabled.
	m_env.push_back("REQUEST_METHOD="+request._method->getType()); //REQUEST_METHOD
	m_env.push_back("REQUEST_URI=/index.php"); //REQUEST_URI
	m_env.push_back("SCRIPT_NAME=/index.php"); //SCRIPT_NAME (virtual path to the file): For example, if you run the script http://www.placeholder.com/cgi-bin/ping.sh and retrieve SCRIPT_NAME, you will get the virtual path of the script or /cgi-bin/ping.sh
	m_env.push_back("SERVER_NAME="+config.getNames().front()); //TODO switch better function call get server_name (localhost)
	m_env.push_back("SERVER_PORT="+socket.getServerPort_Str());
	m_env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	m_env.push_back("SERVER_SOFTWARE=Webserv");
	m_env.push_back("REDIRECT_STATUS=200"); //SI fichier php
	convert.str("");
	convert << socket.GetSocket();
	m_tmpfilename = "/tmp/webserv_" + convert.str() + ".tmp";
	m_c_env = new char*[m_env.size() + 1];
	for (size_t i = 0; i < m_env.size(); i++)
	{
		m_c_env[i] = new char[m_env[i].size() + 1];
		strncpy(m_c_env[i], m_env[i].c_str(), m_env[i].size() + 1);
	}
	m_c_env[m_env.size()] = NULL;
	this->execute(request._body);
}

CGI::CGI(const CGI &src)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CGI::~CGI()
{
	//unlink(m_tmpfilename.c_str());
	for (size_t i = 0; i < m_env.size(); i++)
		delete [] m_c_env[i];
	delete [] m_c_env;
	for (size_t i = 0; i < m_args.size(); i++)
		delete [] m_c_args[i];
	delete [] m_c_args;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

std::ostream &		operator<<( std::ostream & o, CGI const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void				CGI::execute(const std::string & body) //const ?
{
	int		fd;
	pid_t	pid;
	int		status;
	int		pipes[2];

	if ((fd = open(m_tmpfilename.c_str(), O_WRONLY | O_CREAT, 0666)) == -1)
		throw CGI::CGIException("open() failed."); //ERROR 500
	pipe(pipes);
	if (body.empty())
		close(pipes[1]);
	pid = fork();
	switch (pid) {
	case -1:
		close(pipes[0]);
		close(pipes[1]);
		throw CGI::CGIException("fork() failed."); //ERROR 500
	case 0:
		//child
		dup2(pipes[0], STDIN_FILENO);
		dup2(fd, STDOUT_FILENO);
		status = execve(m_c_args[0], m_c_args, m_c_env);
		exit(status);
	default:
		//parent
		if (!body.empty())
			if (write(pipes[1], body.c_str(), body.size()) <= 0)
				Logger::print("Couldn't send Body to CGI.", NULL, ERROR, NORMAL);
		close(pipes[1]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			Logger::print("CGI call was successful.", NULL, SUCCESS, NORMAL);
		else
		{
			Logger::print("CGI call failed.", NULL, ERROR, NORMAL);
			throw CGI::CGIException("execve() failed."); //ERROR 500
		}
		close(pipes[0]);
		close(fd);
	}
}

void	CGI::process(Response& response)
{
	std::ifstream		inputFileStream(m_tmpfilename.c_str());
	std::stringstream	convert;
	std::string			content;
	struct timeval 		tv;
	struct tm			time;
	char				buffer[1024];

	if (!inputFileStream.is_open())
		throw CGI::CGIException("Couldn't read tmp buffer."); //ERROR 500
	convert << inputFileStream.rdbuf();
	content = convert.str();
	inputFileStream.close();
	if (content.find("\r\n\r\n") != std::string::npos)
	{
		convert.str("");
		if (content.find("Status: ") != std::string::npos)
		{
			int status;
			convert << content.substr(8, content.find(" ", 8) - 8);
			convert >> status;
			Logger::print("CGI returned status "+convert.str(), NULL, ERROR, NORMAL);
			response.setCode(status);
			return;
		}
		while (content.find(": ") < content.find("\r\n\r\n"))
		{
			response.addHeader(content.substr(0, content.find(": ") + 1), content.substr(content.find(": ") + 2, content.find("\r\n")));
			content.erase(0, content.find("\r\n"));
			if (content.find(": ") < content.find("\r\n\r\n"))
				content.erase(0, 2);
		}
		content.erase(0, 4);
		convert.str("");
		convert << content.size();
		response.addHeader("Content-Length", convert.str());
		convert.str("");
		struct timeval 	tv;
		gettimeofday(&tv, NULL);
		tv.tv_sec -= 3600; // assume we're GMT+1
		convert << tv.tv_sec;
		strptime(std::string(convert.str()).c_str(), "%s", &time);
		strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", &time);
		response.addHeader("Date", buffer);
		response.addHeader("Server", "Webserv");
		response.setBody("");
		response.setBody(content);
	}
	else
	{
		response.setCode(500);
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */