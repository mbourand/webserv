#ifndef AUTHORIZATION_HEADER_HPP
#define AUTHORIZATION_HEADER_HPP

#include "IHeader.hpp"

class AuthorizationHeader : public IHeader
{
	public:
		AuthorizationHeader();
		AuthorizationHeader(const AuthorizationHeader& other);
		virtual ~AuthorizationHeader();
		AuthorizationHeader& operator=(const AuthorizationHeader& other);

		std::string getType();
		void parse(std::string content);
};

#endif
