/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 19:37:11 by nforay            #+#    #+#             */
/*   Updated: 2021/03/18 16:09:28 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "ServerSocket.hpp"
# include "Logger.hpp"
# include "Request.hpp"
# include "HashTable.hpp"

struct Client {
	ServerSocket	*sckt;
	int				port;
	Request			*req;
};

typedef struct	s_webserv
{
	bool		run;
	HashTable	*file_formatname;
	std::map<int, ServerSocket*> sockets;
}				t_webserv;

void		sighandler(void);

extern t_webserv	g_webserv;

#endif
