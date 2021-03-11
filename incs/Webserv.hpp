/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 19:37:11 by nforay            #+#    #+#             */
/*   Updated: 2021/03/11 19:43:01 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "ServerSocket.hpp"
# include "Logger.hpp"
# include "Request.hpp"

struct Client {
	ServerSocket	*sckt;
	Request			*req;
};

void		sighandler(void);

extern bool g_run;

#endif
