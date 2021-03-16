/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sighandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 19:27:00 by nforay            #+#    #+#             */
/*   Updated: 2021/03/16 16:12:37 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "Webserv.hpp"

static void	sig_int(int sig)
{
	(void)sig;
	signal(SIGINT, SIG_IGN);
	std:: cout << "\b\b" << std::flush;
	Logger::print("Interrupt signal was caught.", NULL, WARNING, SILENT);
	g_webserv.run = false;
	signal(SIGINT, &sig_int);
}

void	sighandler(void)
{
	signal(SIGINT, &sig_int);
}
