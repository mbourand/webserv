/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HashTable.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/15 17:14:00 by nforay            #+#    #+#             */
/*   Updated: 2021/03/15 19:14:36 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASHTABLE_HPP
# define HASHTABLE_HPP

# include <iostream>
# include <string>

typedef struct	s_hnode
{
	std::string	key;
	std::string	value;
	t_hnode		*next;
}				t_hnode;

class HashTable
{

	public:

		HashTable(size_t size);
		HashTable(size_t size, size_t hashdepth);
		~HashTable();

		size_t				Hash(std::string const &str) const;
		bool				Insert(std::string const &key, std::string const &value);
		const std::string &	Lookup(std::string const &key) const;
		bool				Delete(std::string const &key);

		class HashTableException : public std::exception
		{
			public:
				HashTableException() : m_message("HashTable Exception: Can't find given key.") {}
				virtual ~HashTableException() throw() {return ;}
				virtual const char* what() const throw() {return m_message.c_str();}
			
			private:
				std::string const	m_message;
		};

	private:

		t_hnode	**m_table;
		size_t	m_size;
		size_t	m_hashdepth;
};

#endif /* ***************************************************** HASHTABLE_HPP */