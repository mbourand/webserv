#ifndef AUTHORIZATION_HEADER_HPP
#define AUTHORIZATION_HEADER_HPP

#include "Header.hpp"

class AuthorizationHeader : public Header
{
	public:
		AuthorizationHeader();
		AuthorizationHeader(const AuthorizationHeader& other);
		virtual ~AuthorizationHeader();
		AuthorizationHeader& operator=(const AuthorizationHeader& other);

		std::string getType() const;
		Header* clone() const;
};

#endif
