/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcp-client.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/23 19:38:29 by nforay            #+#    #+#             */
/*   Updated: 2021/02/23 19:46:04 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientSocket.hpp"
#include <string>
#include <iostream>

int	main(void)
{
	std::cout << "Starting Client..." << std::endl;
	try
	{
		ClientSocket client("localhost", 8000);
		std::cout << "Client is ready, sending data..." << std::endl;
		try
		{
			std::string	data;
			client << "GET / HTTP/1.1\r\n\r\n";
			std::cout << data << std::endl;
			client >> data;
			std::cout << data << std::endl;
		}
		catch(ClientSocket::ClientSocketException &e)
		{}
	}
	catch(ClientSocket::ClientSocketException &e)
	{
		std::cerr << e.what() << '\n';
	}
	return (0);
}
