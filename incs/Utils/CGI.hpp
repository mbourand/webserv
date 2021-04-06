/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 16:34:07 by nforay            #+#    #+#             */
/*   Updated: 2021/04/07 00:11:29 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include <iostream>
# include <string>
# include <vector>
# include "Request.hpp"
# include "Response.hpp"
# include "ServerSocket.hpp"

class CGI
{

	public:

		CGI(const Request& request, const ConfigContext& config, const ServerSocket& socket, const std::string& realPath);
		CGI(CGI const & src);
		~CGI();

		void	process(Response& response);

		class CGIException : public std::exception
		{
			public:
				CGIException(const char *str, const int code) : m_message(str), m_code(code) {}
				virtual ~CGIException() throw() {return ;}
				virtual const char* what() const throw() {return m_message.c_str();}
				virtual const int code() const throw() {return m_code;}

			private:
				std::string const	m_message;
				int	const			m_code;
		};

	private:

		std::string		find_first_file(const std::string &path, const ConfigContext& config);
		void			execute(const std::string & body);
		int				ParseURI(const Request& req, const ConfigContext& config);

		std::string					m_cgi_path;
		std::string					m_tmpfilename;
		std::string 				m_env_Script_Name;
		std::string 				m_env_Path_Info;
		std::string 				m_env_Path_Translated;
		std::string					m_Document_Root;
		std::string					m_realPath;
		std::vector<std::string>	m_args;
		std::vector<std::string>	m_env;
		char						**m_c_args;
		char						**m_c_env;

};

#endif /* ************************************************************* CGI_H */
