/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HashTable.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/15 17:14:00 by nforay            #+#    #+#             */
/*   Updated: 2021/03/16 03:35:38 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASHTABLE_HPP
# define HASHTABLE_HPP

# include <iostream>
# include <string>
# include <stdint.h>

typedef struct		s_hnode
{
	std::string		key;
	std::string		value;
	struct s_hnode	*next;
}					t_hnode;

class HashTable
{

	public:

		HashTable(unsigned int size);
		HashTable(unsigned int size, unsigned int hashdepth);
		~HashTable();

		uint32_t			Hash(std::string const &str) const;
		bool				Insert(std::string const &key, std::string const &value);
		const std::string &	Lookup(std::string const &key) const;
		t_hnode*			GetNode(std::string const &key) const;
		bool				Delete(std::string const &key);
		void				Print(void) const;

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

		t_hnode			**m_table;
		unsigned int	m_size;
		unsigned int	m_hashdepth;
};

#endif /* ***************************************************** HASHTABLE_HPP */