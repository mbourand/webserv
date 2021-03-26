/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 16:34:07 by nforay            #+#    #+#             */
/*   Updated: 2021/03/25 22:48:02 by nforay           ###   ########.fr       */
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

		CGI(const Request& request, const ConfigContext& config, const ServerSocket& socket);
		CGI(CGI const & src);
		~CGI();

		void	process(Response& response);

		class CGIException : public std::exception
		{
			public:
				CGIException(const char *str) : m_message(str) {}
				virtual ~CGIException() throw() {return ;}
				virtual const char* what() const throw() {return m_message.c_str();}
			
			private:
				std::string const	m_message;
		};

	private:

		void	execute(const std::string & body);

		std::string					m_cgi_path;
		std::string					m_tmpfilename;
		std::vector<std::string>	m_args;
		std::vector<std::string>	m_env;
		char						**m_c_args;
		char						**m_c_env;

};

std::ostream &			operator<<( std::ostream & o, CGI const & i );

#endif /* ************************************************************* CGI_H */