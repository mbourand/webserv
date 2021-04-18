/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 19:37:11 by nforay            #+#    #+#             */
/*   Updated: 2021/04/18 17:30:07 by nforay           ###   ########.fr       */
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

struct Client {
	ServerSocket	*sckt;
	Request			*req;
};

typedef struct						s_webserv
{
	bool							run;
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

	s_webserv();

	void init_config(const std::string& config_path);

	private:
		void init_factories();
}									t_webserv;

void		sighandler(void);

extern t_webserv	g_webserv;

#endif
