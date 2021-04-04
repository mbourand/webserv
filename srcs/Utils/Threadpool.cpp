/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Threadpool.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 19:33:31 by nforay            #+#    #+#             */
/*   Updated: 2021/04/04 02:14:57 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Threadpool.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Threadpool::Threadpool(unsigned int numworkers) : m_numworkers(numworkers)
{
	std::cout << "Threadpool constructor" << std::endl;
	if (!m_numworkers)
		return;
	m_workers.reserve(m_numworkers);
	pthread_mutex_init(&m_jobsmutex, NULL);
	pthread_mutex_lock(&m_jobsmutex);
	for (unsigned int it = 0; it < m_numworkers; it++)
	{
		pthread_t *new_thread = new pthread_t;
		std::cout << "worker created " << it << std::endl;
		pthread_create(new_thread, NULL, worker_entry, this);
		m_workers.push_back(new_thread);
	}
}

Threadpool::Threadpool(const Threadpool &src)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Threadpool::~Threadpool()
{
	//pthread_mutex_destroy(&m_jobsmutex);
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Threadpool &				Threadpool::operator=(Threadpool const &rhs)
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<(std::ostream &o, Threadpool const &i)
{
	//o << "Value = " << i.getValue();
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void					Threadpool::Lock(void)
{
	std::cout << "worker Lock" << std::endl;
	pthread_mutex_lock(&m_jobsmutex);
}

void					Threadpool::Unlock(void)
{
	std::cout << "worker Unlock" << std::endl;
	pthread_mutex_unlock(&m_jobsmutex);
}

void					Threadpool::AddJob(Client &client, bool type)
{
	if (type)
		std::cout << "response job added" << std::endl;
	else
		std::cout << "request job added" << std::endl;
	Job *job = new Job;
	job->client = &client;
	job->type = type;
	m_jobs.push_front(job);
	Unlock();
}

void					*Threadpool::WaitForWork(void *)
{
	std::cout << "worker WaitForWork" << std::endl;
	while (true)
	{
		Lock();
		if (!g_webserv.run)
			return NULL;
		if (m_jobs.size())
		{
			std::cout << "worker taking job" << std::endl;
			Job *job = m_jobs.back();
			m_jobs.pop_back();
			if (job->type)
				handle_server_response(*job->client);
			else
				handle_client_request(*job->client);
			delete job;
		}
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */