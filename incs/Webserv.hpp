/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 19:37:11 by nforay            #+#    #+#             */
/*   Updated: 2021/05/03 21:53:52 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <list>
# include "VirtualHost.hpp"
# include "ServerSocket.hpp"
# include "Logger.hpp"
# include "Request.hpp"
# include "HashTable.hpp"
# include "Headers.h"
# include "Credentials.hpp"

struct Client {
	ServerSocket	*sckt;
	Request			*req;
};

class Credentials;

typedef struct						s_webserv
{
	bool							run;
	bool							debug;
	HashTable						*file_formatname;
	std::map<int, ServerSocket*>	sockets;
	std::string						cwd;
	Factory<IMethod*>				methods;
	Factory<Header*>				headers;
	std::list<VirtualHost>			vhosts;
	unsigned int					workers_amount;
	unsigned int					max_connections;
	bool							compression_deflate;
	bool							compression_gzip;
	unsigned int					compression_level;
	Credentials						*creds;

	s_webserv();

	void init_config(const std::string& config_path);

	private:
		void init_factories();
}									t_webserv;

void		sighandler(void);

extern t_webserv	g_webserv;

#endif
