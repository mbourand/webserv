/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Threadpool.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 19:33:31 by nforay            #+#    #+#             */
/*   Updated: 2021/04/05 00:46:12 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADPOOL_HPP
# define THREADPOOL_HPP

# include <iostream>
# include <string>
# include <deque>
# include <vector>
# include <pthread.h>
# include "Webserv.hpp"
# include "Logger.hpp"

bool	handle_server_response(Client &client); //temp
bool	handle_client_request(Client &client); //temp

class Threadpool
{

	public:

		Threadpool(unsigned int numworkers);
		~Threadpool();

		bool	AddJob(Client &client);
		void	*WaitForWork(void *);
		static void	*worker_entry(void *context){return (((Threadpool *)context)->WaitForWork(context));}

		Threadpool &		operator=(Threadpool const &rhs);

	private:

		void	Lock(void);
		void	Unlock(void);

		std::vector<pthread_t*>	m_workers;
		std::deque<Client*>			m_jobs;
		unsigned int			m_numworkers;
		pthread_mutex_t			m_jobsmutex;
		size_t					m_maxqueuedjobs;

};

#endif /* ****************************************************** THREADPOOL_H */