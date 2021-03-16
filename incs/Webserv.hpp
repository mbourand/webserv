/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 19:37:11 by nforay            #+#    #+#             */
/*   Updated: 2021/03/16 16:04:47 by nforay           ###   ########.fr       */
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
	Request			*req;
};

typedef struct	s_webserv
{
	bool		run;
	HashTable	*file_formatname;
}				t_webserv;

void		sighandler(void);

extern t_webserv	g_webserv;

#endif
