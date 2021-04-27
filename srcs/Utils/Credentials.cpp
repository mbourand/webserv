/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Credentials.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/27 01:39:22 by nforay            #+#    #+#             */
/*   Updated: 2021/04/27 03:16:26 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <iostream>
#include "Credentials.hpp"
#include "Base64.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Credentials::Credentials()
{
	for (std::list<VirtualHost>::iterator it = g_webserv.vhosts.begin(); it != g_webserv.vhosts.end(); it++)
	{
		const std::string	&path = it->getConfig().getParam("auth_basic_user_file").front();
		if (!path.empty() && !Is_Cache_Uptodate(path))
			Parse_user_file(path);
	}
}

Credentials::Credentials(const Credentials &src)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Credentials::~Credentials()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Credentials &				Credentials::operator=(Credentials const &rhs)
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void						Credentials::Parse_user_file(const std::string &path)
{
	Authentication_file		user_file;
	struct stat 			stats;

	if (lstat(path.c_str(), &stats) < 0 || !S_ISREG(stats.st_mode) || user_file.last_modified == stats.st_mtime)
		return ;
	std::ifstream 			inputFileStream(path.c_str());
	if (!inputFileStream.is_open())
		return ;
	std::string				line;
	while (!inputFileStream.eof()) {
		std::getline(inputFileStream, line);
		if (!line.empty())
			user_file.content.push_back(base64_encode(line.c_str(), line.size()));
	}
	user_file.file_path = path;
	user_file.last_modified = stats.st_mtime;
	m_credentials.push_back(user_file);
}

void						Credentials::Update_user_file(const std::string &path)
{
	for (std::list<Authentication_file>::iterator it = m_credentials.begin(); it != m_credentials.end(); it++)
		if (it->file_path == path)
			it = m_credentials.erase(it);
	Parse_user_file(path);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

bool						Credentials::Is_Cached(const std::string &path) const
{
	for (std::list<Authentication_file>::const_iterator it = m_credentials.begin(); it != m_credentials.end(); it++)
		if (path == it->file_path)
			return (true);
	return (false);
}

bool						Credentials::Is_Cache_Uptodate(const std::string &path) const
{
	struct stat 			stats;
	if (lstat(path.c_str(), &stats) < 0)
		return (false);
	for (std::list<Authentication_file>::const_iterator it = m_credentials.begin(); it != m_credentials.end(); it++)
		if (path == it->file_path && it->last_modified == stats.st_mtime)
			return (true);
	return (false);
}

bool						Credentials::Check_Credentials(const std::string &path, const std::string &credentials)
{
	if (!Is_Cache_Uptodate(path))
		Update_user_file(path);
	for (std::list<Authentication_file>::iterator it = m_credentials.begin(); it != m_credentials.end(); it++)
	{
		if (path == it->file_path)
			for (std::vector<std::string>::iterator ita = it->content.begin(); ita != it->content.end(); ita++)
			{
				if (credentials == *ita)
					return (true);
			}
	}
	return (false);
}

/* ************************************************************************** */