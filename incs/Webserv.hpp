/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbourand <mbourand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 19:37:11 by nforay            #+#    #+#             */
/*   Updated: 2021/04/05 03:51:50 by mbourand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "ServerSocket.hpp"
# include "Logger.hpp"
# include "Request.hpp"
# include "HashTable.hpp"
# include "Headers.h"

struct Client {
	ServerSocket	*sckt;
	Request			*req;
};

typedef struct	s_webserv
{
	bool		run;
	HashTable	*file_formatname;
	std::map<int, ServerSocket*> sockets;
	std::string cwd;
	Factory<IMethod*> methods;
	Factory<Header*> headers;
}				t_webserv;

void		sighandler(void);

extern t_webserv	g_webserv;

#endif
