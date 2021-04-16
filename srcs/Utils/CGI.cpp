/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbourand <mbourand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 16:34:07 by nforay            #+#    #+#             */
/*   Updated: 2021/04/16 15:12:36 by mbourand         ###   ########.fr       */
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

/**
 * !Note : CGI n'a pas besoin de vérifier que realPath contient une extension, c'est déjà vérifié dans les methods
 */

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/



CGI::CGI(const Request& request, const ConfigContext& config, const ServerSocket& socket, const std::string& realPath)
	: m_realPath(realPath)
{
	std::ostringstream	convert;
	int					code;
	Header				*auth_type = NULL;
	Header				*content_type = NULL;
	std::string			extension;

	if ((code = ParseURI(request, config)))
		throw CGI::CGIException("ParseURI failed.", code);
	for (Request::HeadersVector::const_iterator it = request._headers.begin(); it != request._headers.end(); it++)
	{
		if ((*it)->getType() == WWWAuthenticateHeader().getType())
			auth_type = *it;
		else if ((*it)->getType() == ContentTypeHeader().getType())
			content_type = *it;
	}
	std::string document_root = ft::simplify_path(config.getParam("root").front(), false, 0);
	if (ft::contains(m_env_Script_Name, '.'))
		extension = m_env_Script_Name.substr(m_env_Script_Name.find('.'));
	if (!extension.empty() && config.getCGIExtensions().find(extension) != config.getCGIExtensions().end())
	{
		m_args.push_back(config.getCGIExtensions().find(extension)->second); // c'est le chemin vers l'exécutable cgi
		m_args.push_back(document_root + m_env_Script_Name);
		if (extension == ".php")
			m_env.push_back("REDIRECT_STATUS=200");
	}
	else
	{
		m_args.push_back(document_root + m_env_Script_Name);
		if (!m_env_Path_Info.empty())
			m_args.push_back(document_root + m_env_Path_Info);
	}
	if (auth_type)
		m_env.push_back("AUTH_TYPE="+auth_type->getValue());
	if (request._method->requestHasBody() && !request._body.empty())
	{
		convert << request._body.size();
		m_env.push_back("CONTENT_LENGTH="+convert.str());
		if (content_type)
			m_env.push_back("CONTENT_TYPE="+content_type->getValue());
		else
			m_env.push_back("CONTENT_TYPE=US-ASCII");
	}
	m_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	if (!m_env_Path_Info.empty())
		m_env.push_back("PATH_INFO="+m_env_Path_Info);
	else if (m_env_Script_Name.find("ubuntu_cgi_tester") != std::string::npos)
		m_env.push_back("PATH_INFO="+m_env_Script_Name);
	if (document_root.find("..") != std::string::npos)
	{
		chdir(document_root.substr(0, document_root.find_last_of("/")).c_str());
		document_root = ft::get_cwd();
		chdir(g_webserv.cwd.c_str());
	}
	if (document_root[document_root.size() - 1] == '/')
		document_root.erase(document_root.size() - 1);
	m_env.push_back("PATH_TRANSLATED=" + document_root + m_env_Script_Name);
	if (!request._url._query.empty())
		m_env.push_back("QUERY_STRING="+request._url._query);
	m_env.push_back("REMOTE_ADDR="+socket.getIPAddress());
	//m_env.push_back("REMOTE_IDENT=something"); //REMOTE_IDENT This variable stores the user ID running the CGI script. The user ID is stored only if the ident process is running since ident returns a response containing not only user ID information, but also the name of the OS running the script.
	//if ((header_found = request._headerFactory.getByType(WWWAuthenticateHeader().getType())) != NULL)
	//	m_env.push_back("REMOTE_USER=something"); //REMOTE_USER Querying the REMOTE_USER variable will give the user name information of the entity making the request. This is only valid if authentication is enabled.
	m_env.push_back("REQUEST_METHOD="+request._method->getType());
	m_env.push_back("REQUEST_URI="+request._url._path);
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
	this->execute(request._body);
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CGI::~CGI()
{
	unlink(m_tmpfilename.c_str());
}

/*
** --------------------------------- METHODS ----------------------------------
*/

std::string		CGI::find_first_file(const std::string &, const ConfigContext& config)
{
	struct stat	file_stats;
	size_t		start = 0;
	size_t		end = 0;

	std::string config_root = config.getParam("root").front();
	std::string url_after_root = m_realPath.substr(m_realPath.rfind(config_root) + config_root.size());
	std::string url_until_root = m_realPath.substr(0, m_realPath.rfind(config_root) + config_root.size());
	std::string extension;
	if (url_until_root.empty())
		url_until_root = "/";

	end = url_after_root.find("/", end);

	while (end != std::string::npos)
	{
		if (lstat((url_until_root + url_after_root.substr(start, end)).c_str(), &file_stats) < 0)
			break;
		if (url_after_root.rfind(".") != std::string::npos)
			extension = url_after_root.substr(url_after_root.find('.'));
		if ((!extension.empty() && config.getCGIExtensions().find(extension) != config.getCGIExtensions().end() && S_ISREG(file_stats.st_mode)) || S_ISREG(file_stats.st_mode))
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
	std::string	cgi_path = config.getParam("cgi-dir").front(); //replace with config->getparam

	m_env_Script_Name = find_first_file(req._url._path, config);
	if (m_env_Script_Name.empty())
		return (404);
	m_env_Path_Info = m_realPath.substr(m_realPath.find(m_env_Script_Name) + m_env_Script_Name.size());
	if (m_env_Path_Info.empty())
		m_env_Path_Info = "";
	else if (m_env_Path_Info[m_env_Path_Info.size() - 1] == '/')
		m_env_Path_Info.erase(m_env_Path_Info.size() - 1);
	return (0);
}

void				CGI::execute(const std::string & body)
{
	int		fd;
	pid_t	pid;
	int		status;
	int		pipes[2];

	if (pipe(pipes) < 0)
	{
		close(fd);
		throw CGI::CGIException("pipe() failed.", 500);
	}
	pid = fork();
	switch (pid) {
	case -1:
		close(pipes[0]);
		close(pipes[1]);
		//close(fd);
		throw CGI::CGIException("fork() failed.", 500);
	case 0:
		if ((fd = open(m_tmpfilename.c_str(), O_WRONLY | O_CREAT, 0666)) == -1)
			exit(1);
		close(pipes[1]); //close writing end query pipe
		if (dup2(pipes[0], STDIN_FILENO) < 0) //dup reading end query pipe to STDIN
			exit(1);
		if (dup2(fd, STDOUT_FILENO) < 0) //dup stdout to tmp file
			exit(1);
		VectorToArray();
		if (execve(m_c_args[0], m_c_args, m_c_env) < 0)
			exit (1);
	default:
		close(pipes[0]); // close reading end query pipe
		if (!body.empty())
			if (write(pipes[1], body.c_str(), body.size()) <= 0) // input writing end query pipe
			{
				Logger::print("Couldn't send Body to CGI.", NULL, ERROR, NORMAL);
				kill(pid, SIGINT);
			}
		close(pipes[1]);
		waitpid(pid, &status, 0);
		//close(fd);
		if (WIFEXITED(status))
			Logger::print("CGI execution was successful.", NULL, SUCCESS, VERBOSE);
		else
			throw CGI::CGIException("CGI execution failed.", 500);
	}
}

void	CGI::VectorToArray(void)
{
	m_c_env = new char*[m_env.size() + 1];
	for (size_t i = 0; i < m_env.size(); i++)
	{
		m_c_env[i] = new char[m_env[i].length() + 1];
		ft::strncpy(m_c_env[i], m_env[i].c_str(), m_env[i].size() + 1);
	}
	m_c_env[m_env.size()] = NULL;
	m_c_args = new char*[m_args.size() + 1];
	for (size_t i = 0; i < m_args.size(); i++)
	{
		m_c_args[i] = new char[m_args[i].size() + 1];
		ft::strncpy(m_c_args[i], m_args[i].c_str(), m_args[i].size() + 1);
	}
	m_c_args[m_args.size()] = NULL;
}


void	CGI::process(Response& response)
{
	std::ifstream		inputFileStream(m_tmpfilename.c_str());
	std::stringstream	convert;
	std::string			content;
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
