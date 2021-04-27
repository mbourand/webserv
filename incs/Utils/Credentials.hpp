/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Credentials.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/27 01:39:23 by nforay            #+#    #+#             */
/*   Updated: 2021/04/27 03:08:46 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CREDENTIALS_HPP
# define CREDENTIALS_HPP

# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <list>
# include <vector>
# include <string>
# include "Webserv.hpp"

struct Authentication_file {
	std::string 				file_path;
	std::vector<std::string>	content;
	time_t						last_modified;
};

class Credentials
{

	public:

		Credentials();
		Credentials(Credentials const &src);
		~Credentials();

		void				Parse_user_file(const std::string &path);
		void				Update_user_file(const std::string &path);

		bool				Is_Cached(const std::string &path) const;
		bool				Is_Cache_Uptodate(const std::string &path) const;
		bool				Check_Credentials(const std::string &path, const std::string &credentials);

		Credentials&		operator=(Credentials const &rhs);

	private:

		std::list<Authentication_file>	m_credentials;

};

#endif /* *************************************************** CREDENTIALS_HPP */