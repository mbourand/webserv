#ifndef WWW_AUTHENTICATE_HEADER_HPP
#define WWW_AUTHENTICATE_HEADER_HPP

#include "Header.hpp"

class WWWAuthenticateHeader : public Header
{
	public:
		WWWAuthenticateHeader();
		WWWAuthenticateHeader(const WWWAuthenticateHeader& other);
		virtual ~WWWAuthenticateHeader();
		WWWAuthenticateHeader& operator=(const WWWAuthenticateHeader& other);

		std::string getType() const;
		Header* clone() const;
};

#endif
