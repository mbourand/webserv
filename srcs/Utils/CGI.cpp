/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbourand <mbourand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 16:34:07 by nforay            #+#    #+#             */
/*   Updated: 2021/04/08 20:14:54 by mbourand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sstream>
#include <fstream>
#include "CGI.hpp"
#include "Logger.hpp"
#include "ServerSocket.hpp"
#include "Webserv.hpp"
#include "Utils.hpp"

#include <string.h>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CGI::CGI(const Request& request, const ConfigContext& config, const ServerSocket& socket, const std::string& realPath)
	: m_realPath(realPath)
{
	Header				*header_found;
	std::ostringstream	convert;
	int					code;
	char	*buffer;

	if ((code = ParseURI(request, config)))
		throw CGI::CGIException("ParseURI failed.", code);
	std::string document_root = ft::simplify_path(config.getParamPath("root", request._path).front(), false, 0);
	std::string extension = m_env_Script_Name.substr(m_env_Script_Name.find('.'));
	if (config.getCGIExtensionsPath(request._path).find(extension) != config.getCGIExtensionsPath(request._path).end())
	{
		m_args.push_back(config.getCGIExtensionsPath(request._path).find(extension)->second); // c'est le chemin vers l'exécutable cgi
		m_args.push_back(document_root + m_env_Script_Name);
		m_env.push_back("REDIRECT_STATUS=200");
	}
	else
	{
		m_args.push_back(document_root + m_env_Script_Name);
		if (!m_env_Path_Info.empty())
			m_args.push_back(document_root + m_env_Path_Info);
	}
	m_c_args = new char*[m_args.size() + 1];
	for (size_t i = 0; i < m_args.size(); i++)
	{
		m_c_args[i] = new char[m_args[i].size() + 1];
		ft::strncpy(m_c_args[i], m_args[i].c_str(), m_args[i].size() + 1);
	}
	m_c_args[m_args.size()] = NULL;
	if ((header_found = g_webserv.headers.getByType(WWWAuthenticateHeader().getType())) != NULL)
		m_env.push_back("AUTH_TYPE="+header_found->getValue());
	if (request._method->requestHasBody() && !request._body.empty())
	{
		convert << request._body.size();
		m_env.push_back("CONTENT_LENGTH="+convert.str());
		if ((header_found = g_webserv.headers.getByType(ContentTypeHeader().getType())) != NULL)
			m_env.push_back("CONTENT_TYPE="+header_found->getValue());
		else
			m_env.push_back("CONTENT_TYPE=US-ASCII");
	}
	m_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	if (!m_env_Path_Info.empty())
		m_env.push_back("PATH_INFO="+m_env_Path_Info);
	else
		m_env.push_back("PATH_INFO="+m_env_Script_Name);
	if (document_root.find("..") != std::string::npos)
	{
		chdir(document_root.substr(0, document_root.find_last_of("/")).c_str());
		document_root = ft::get_cwd();
		chdir(g_webserv.cwd.c_str());
	}
	m_env.push_back("PATH_TRANSLATED=" + document_root + m_env_Script_Name);
	if (!request._query_string.empty())
		m_env.push_back("QUERY_STRING="+request._query_string);
	m_env.push_back("REMOTE_ADDR="+socket.getIPAddress());
	//m_env.push_back("REMOTE_IDENT=something"); //REMOTE_IDENT This variable stores the user ID running the CGI script. The user ID is stored only if the ident process is running since ident returns a response containing not only user ID information, but also the name of the OS running the script.
	//if ((header_found = request._headerFactory.getByType(WWWAuthenticateHeader().getType())) != NULL)
	//	m_env.push_back("REMOTE_USER=something"); //REMOTE_USER Querying the REMOTE_USER variable will give the user name information of the entity making the request. This is only valid if authentication is enabled.
	m_env.push_back("REQUEST_METHOD="+request._method->getType());
	m_env.push_back("REQUEST_URI="+request._path);
	m_env.push_back("SCRIPT_NAME="+m_env_Script_Name);
	m_env.push_back("SERVER_NAME="+config.getNames().front()); //TODO switch better function call get server_name (localhost)
	m_env.push_back("SERVER_PORT="+socket.getServerPort_Str());
	m_env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	m_env.push_back("SERVER_SOFTWARE=Webserv");
	convert.str("");
	convert << socket.GetSocket();
	m_tmpfilename = "/tmp/webserv_" + convert.str() + "-";
	convert.str("");
	struct timeval	tv;
	gettimeofday(&tv, NULL);
	convert << tv.tv_usec;
	m_tmpfilename += convert.str() + ".tmp";
	m_c_env = new char*[m_env.size() + 1];
	for (size_t i = 0; i < m_env.size(); i++)
	{
		m_c_env[i] = new char[m_env[i].length() + 1];
		ft::strncpy(m_c_env[i], m_env[i].c_str(), m_env[i].size() + 1);
	}
	m_c_env[m_env.size()] = NULL;
	this->execute(request._body);
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CGI::~CGI()
{
	unlink(m_tmpfilename.c_str());
	for (size_t i = 0; i < m_env.size(); i++)
		delete [] m_c_env[i];
	delete [] m_c_env;
	for (size_t i = 0; i < m_args.size(); i++)
		delete [] m_c_args[i];
	delete [] m_c_args;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

std::string		CGI::find_first_file(const std::string &path, const ConfigContext& config)
{
	struct stat	file_stats;
	size_t		start = 0;
	size_t		end = 0;

	std::string config_root = config.getParamPath("root", path).front();
	std::string url_after_root = m_realPath.substr(m_realPath.rfind(config_root) + config_root.size());
	std::string url_until_root = m_realPath.substr(0, m_realPath.rfind(config_root) + config_root.size());
	if (url_until_root.empty())
		url_until_root = "/";

	end = url_after_root.find("/", end);

	while (end != std::string::npos)
	{
		if (lstat((url_until_root + url_after_root.substr(start, end)).c_str(), &file_stats) < 0)
			break;
		std::string extension = url_after_root.substr(url_after_root.find('.'));
		if ((config.getCGIExtensionsPath(path).find(extension) != config.getCGIExtensionsPath(path).end() && S_ISREG(file_stats.st_mode)) || S_ISREG(file_stats.st_mode))
			return (url_after_root.substr(start, end));
		else if (S_ISDIR(file_stats.st_mode))
		{
			if ((end = url_after_root.find("/", end + 1)) == std::string::npos)
				end = url_after_root.length();
		}
		else
			break;
	}
	return ("");
}

int	CGI::ParseURI(const Request& req, const ConfigContext& config)
{
	size_t	start = 0;
	size_t	end = 0;
	std::string	cgi_path = config.getParamPath("cgi-dir", req._path).front(); //replace with config->getparam

	m_env_Script_Name = find_first_file(req._path, config);
	if (m_env_Script_Name.empty())
		return (404);
	m_env_Path_Info = m_realPath.substr(m_realPath.find(m_env_Script_Name) + m_env_Script_Name.size());
	if (m_env_Path_Info.empty())
		m_env_Path_Info = "";
	return (0);
}

void				CGI::execute(const std::string & body)
{
	int		fd;
	pid_t	pid;
	int		status;
	int		pipes[2];

	if ((fd = open(m_tmpfilename.c_str(), O_WRONLY | O_CREAT, 0666)) == -1)
		throw CGI::CGIException("open() failed.", 500);
	if (pipe(pipes) < 0)
		throw CGI::CGIException("pipe() failed.", 500);
	if (body.empty())
		close(pipes[1]);
	pid = fork();
	switch (pid) {
	case -1:
		close(pipes[0]);
		close(pipes[1]);
		close(fd);
		throw CGI::CGIException("fork() failed.", 500);
	case 0:
		close(pipes[1]);
		dup2(pipes[0], STDIN_FILENO);
		dup2(fd, STDOUT_FILENO);
		status = execve(m_c_args[0], m_c_args, m_c_env);
		exit(status);
	default:
		close(pipes[0]);
		if (!body.empty())
			if (write(pipes[1], body.c_str(), body.size()) <= 0)
			{
				Logger::print("Couldn't send Body to CGI.", NULL, ERROR, NORMAL);
				kill(pid, SIGINT);
			}
		waitpid(pid, &status, 0);
		close(pipes[1]);
		close(fd);
		if (WIFEXITED(status))
			Logger::print("CGI execution was successful.", NULL, SUCCESS, NORMAL);
		else
			throw CGI::CGIException("CGI execution failed.", 500);
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
	int					status;

	if (!inputFileStream.is_open())
		throw CGI::CGIException("Couldn't read tmp buffer.", 500);
	convert << inputFileStream.rdbuf();
	content = convert.str();
	inputFileStream.close();
	if (content.find("\r\n\r\n") != std::string::npos)
	{
		convert.str("");
		if (content.find("Status: ") != std::string::npos)
		{
			convert << content.substr(8, content.find(" ", 8) - 8);
			convert >> status;
			if (status != 200)
			{
				Logger::print("CGI returned status "+convert.str(), NULL, ERROR, NORMAL);
				response.setCode(status);
				return;
			}
		}
		while (content.find(": ") < content.find("\r\n\r\n"))
		{
			size_t found = content.find(": ");
			response.addHeader(content.substr(0, found), content.substr(found + 2, (content.find("\r\n")) - found - 2));
			content.erase(0, content.find("\r\n"));
			if (content.find(": ") < content.find("\r\n\r\n"))
				content.erase(0, 2);
		}
		content.erase(0, 4);
		convert.clear();
		convert.str("");
		convert << content.size();
		response.addHeader("Content-Length", convert.str());
		response.addDateHeader();
		response.addHeader("Server", "Webserv");
		response.setBody(content);
	}
	else
	{
		Logger::print("CGI produced invalid headers.", NULL, ERROR, NORMAL);
		response.setCode(500);
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
