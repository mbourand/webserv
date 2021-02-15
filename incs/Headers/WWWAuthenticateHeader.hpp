#ifndef WWW_AUTHENTICATE_HEADER_HPP
#define WWW_AUTHENTICATE_HEADER_HPP

#include "IHeader.hpp"

class WWWAuthenticateHeader : public IHeader
{
	public:
		WWWAuthenticateHeader();
		WWWAuthenticateHeader(const WWWAuthenticateHeader& other);
		virtual ~WWWAuthenticateHeader();
		WWWAuthenticateHeader& operator=(const WWWAuthenticateHeader& other);

		std::string getType();
		void parse(std::string content);
};

#endif
