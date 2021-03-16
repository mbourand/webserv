/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HashTable.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/15 17:14:00 by nforay            #+#    #+#             */
/*   Updated: 2021/03/16 03:39:08 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HashTable.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

HashTable::HashTable(unsigned int size) : m_size(size), m_hashdepth(4)
{
	if (!m_size)//Y U DO DIS
		this->m_size = 1;
	this->m_table = new t_hnode*[m_size];
	for (unsigned int i = 0; i < this->m_size; i++)
	{
		this->m_table[i] = NULL;
	}
}

HashTable::HashTable(unsigned int size, unsigned int hashdepth) : m_size(size), m_hashdepth(hashdepth)
{
	if (!m_size)
		m_size = 1;
	if (!m_hashdepth)
		m_hashdepth = 4;
	this->m_table = new t_hnode*[m_size];
	for (unsigned int i = 0; i < this->m_size; i++)
	{
		this->m_table[i] = NULL;
	}
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

HashTable::~HashTable()
{
	t_hnode *node;
	t_hnode *tmp;
	for (unsigned int i = 0; i < this->m_size; i++)
	{
		node = this->m_table[i];
		while (node != NULL)
		{
			tmp = node;
			node = node->next;
			delete tmp;
		}
	}
	delete [] this->m_table;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

uint32_t				HashTable::Hash(std::string const &str) const
{
	uint32_t					hash = 0;
	unsigned int				i = 0;
	unsigned int				j = this->m_hashdepth;
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
	return (hash % this->m_size);
}

bool					HashTable::Insert(std::string const &key, std::string const &value)
{
	if (key.empty() || value.empty() || this->GetNode(key))
		return (false);
	t_hnode 		*newnode;
	unsigned int	index = this->Hash(key);

	newnode = new t_hnode;
	newnode->key = key;
	newnode->value = value;
	newnode->next = this->m_table[index];
	this->m_table[index] = newnode;
	return (true);
}

bool					HashTable::Delete(std::string const &key)
{
	unsigned int index = this->Hash(key);
	t_hnode *node = this->m_table[index];
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
			this->m_table[index] = node->next;
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

void					HashTable::Print(void) const
{
	t_hnode *node;
	for (unsigned int i = 0; i < this->m_size; i++)
	{
		node = this->m_table[i];
		if (node != NULL)
			std::cout << i << ":\n\t";
		while (node != NULL)
		{
			std::cout << node->key << "=" << node->value << " ";
			node = node->next;
		}
		if (!node)
			std::cout << std::endl;
	}
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

t_hnode*				HashTable::GetNode(std::string const &key) const
{
	t_hnode *node = this->m_table[this->Hash(key)];
	while (node != NULL && node->key != key)
		node = node->next;
	return (node);
}

/* ************************************************************************** */