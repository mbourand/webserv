/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HashTable.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/15 17:14:00 by nforay            #+#    #+#             */
/*   Updated: 2021/03/15 19:24:12 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HashTable.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

HashTable::HashTable(size_t size) : m_size(size), m_hashdepth(4)
{
	if (!m_size)//Y U DO DIS
		m_size = 1;
	*this->m_table = new t_hnode[m_size];
}

HashTable::HashTable(size_t size, size_t hashdepth) : m_size(size), m_hashdepth(hashdepth)
{
	if (!m_size)
		m_size = 1;
	if (!m_hashdepth)
		m_hashdepth = 4;
	*this->m_table = new t_hnode[m_size];
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

HashTable::~HashTable()
{
	//TODO delete nodes;
	delete [] this->m_table;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

size_t					HashTable::Hash(std::string const &str) const
{
	size_t						hash = 0;
	size_t						i = 0;
	size_t						j = this->m_hashdepth;
	std::string::const_iterator	it = str.begin();

	while (it != str.end() && j)// one_at_a_time hash
	{
		hash += *it++;
		hash += hash << 10;
		hash ^= hash >> 6;
		j--;
	}
	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;
	return (hash & (this->m_size - 1));
}

bool					HashTable::Insert(std::string const &key, std::string const &value)
{
	t_hnode *newnode;

	if (key.empty() || value.empty())
		return (false);
	newnode = new t_hnode;
	newnode->key = key;
	newnode->value = value;
	newnode->next = this->m_table[this->Hash(key)];
	m_table[this->Hash(key)] = newnode;
	return (true);
}

bool					HashTable::Delete(std::string const &key)
{
	t_hnode *node = this->m_table[this->Hash(key)];
	t_hnode *prevnode = NULL;

	while (node != NULL && node->key != key)
	{
		prevnode = node;
		node = prevnode->next;
	}
	if (node != NULL && node->key == key)
	{
		if (prevnode == NULL)
		{
			prevnode = node;
			this->m_table[this->Hash(key)] = node->next;
			delete prevnode;
		}
		else
		{
			prevnode->next = node->next;
			delete node;
		}
	}
	return (false);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

const std::string &		HashTable::Lookup(std::string const &key) const
{
	t_hnode *node = this->m_table[this->Hash(key)];
	while (node != NULL && node->key != key)
		node = node->next;
	if (node == NULL)
		throw HashTable::HashTableException();
	return (node->value);
}

/* ************************************************************************** */