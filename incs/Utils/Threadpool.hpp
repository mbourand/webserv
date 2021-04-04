/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Threadpool.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 19:33:31 by nforay            #+#    #+#             */
/*   Updated: 2021/04/04 02:10:33 by nforay           ###   ########.fr       */
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

bool	handle_server_response(Client &client); //temp
bool	handle_client_request(Client &client); //temp

struct Job {
	Client				*client;
	bool				type;
};

class Threadpool
{

	public:

		Threadpool(unsigned int numworkers);
		Threadpool(Threadpool const &src);
		~Threadpool();

		void	AddJob(Client &client, bool type);
		void	*WaitForWork(void *);
		static void	*worker_entry(void *context){std::cout << "worker waiting" << std::endl;return (((Threadpool *)context)->WaitForWork(context));}

		Threadpool &		operator=(Threadpool const &rhs);

	private:

		void	Lock(void);
		void	Unlock(void);

		std::vector<pthread_t*>	m_workers;
		std::deque<Job*>		m_jobs;
		unsigned int			m_numworkers;
		pthread_mutex_t			m_jobsmutex;

};

std::ostream &			operator<<(std::ostream &o, Threadpool const &i);

#endif /* ****************************************************** THREADPOOL_H */