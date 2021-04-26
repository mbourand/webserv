/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Threadpool.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 19:33:31 by nforay            #+#    #+#             */
/*   Updated: 2021/04/26 17:43:42 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Threadpool.hpp"
#include <unistd.h>



/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/



Threadpool::Threadpool(unsigned int numworkers) : m_numworkers(numworkers), m_maxqueuedjobs(100)
{
	if (!m_numworkers)
		return;
	Logger::print("Populating Threadpool with workers...", NULL, INFO, SILENT);
	m_workers.reserve(m_numworkers);
	pthread_mutex_init(&m_jobsmutex, NULL);
	for (unsigned int it = 0; it < m_numworkers; it++)
	{
		pthread_t *new_thread = new pthread_t;
		pthread_create(new_thread, NULL, worker_entry, this);
		pthread_detach(*new_thread);
		m_workers.push_back(new_thread);
	}
}



/*
** -------------------------------- DESTRUCTOR --------------------------------
*/



Threadpool::~Threadpool()
{
	if (!m_numworkers)
		return;
	while (m_currentjobs.size())
		usleep(1);
	for (unsigned int it = 0; it < m_numworkers; it++)
		delete m_workers[it];
	Client *job;
	while (m_jobs.size() && (job = m_jobs.back()))
	{
		delete job->req;
		delete job;
		m_jobs.pop_back();
	}
	pthread_mutex_destroy(&m_jobsmutex);
}



/*
** --------------------------------- OVERLOAD ---------------------------------
*/



Threadpool &				Threadpool::operator=(Threadpool const &rhs)
{
	if ( this != &rhs )
	{
		this->m_numworkers = rhs.m_numworkers;
		this->m_workers = rhs.m_workers;
		this->m_jobs = rhs.m_jobs;
		this->m_jobsmutex = rhs.m_jobsmutex;
		this->m_maxqueuedjobs = rhs.m_maxqueuedjobs;
	}
	return *this;
}



/*
** --------------------------------- METHODS ----------------------------------
*/



void					Threadpool::Lock(void)
{
	if (!m_numworkers)
		return;
	pthread_mutex_lock(&m_jobsmutex);
}

void					Threadpool::Unlock(void)
{
	if (!m_numworkers)
		return;
	pthread_mutex_unlock(&m_jobsmutex);
}

bool					Threadpool::AddJob(Client &client)
{
	if (!m_numworkers || (m_jobs.size() > m_maxqueuedjobs))
		return(handle_server_response(client));
	Client *threaded_client = new Client;
	threaded_client->sckt = client.sckt;
	Lock();
	threaded_client->req = client.req;
	client.req = new Request(client.sckt->getServerPort());
	m_jobs.push_front(threaded_client);
	Unlock();
	return (false);
}

void					*Threadpool::WaitForWork(void *)
{
	while (g_webserv.run)
	{
		Lock();
		if (!g_webserv.run)
			return NULL;
		if (m_jobs.size())
		{
			Client *job = m_jobs.back();
			m_jobs.pop_back();
			m_currentjobs.push_back(job);
			Unlock();
			handle_server_response(*job);
			Lock();
			m_currentjobs.remove(job);
			delete job->req;
			delete job;
			Unlock();
		}
		else
		{
			Unlock();
			usleep(100 * m_numworkers);
		}
	}
	return (NULL);
}



/*
** --------------------------------- ACCESSOR ---------------------------------
*/



const std::deque<Client*>& Threadpool::getJobs() const { return m_jobs; }
const std::list<Client*>& Threadpool::getCurrentJobs() const { return m_currentjobs; }

/* ************************************************************************** */
