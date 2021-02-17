#ifndef SERVER_HEADER_HPP
#define SERVER_HEADER_HPP

#include "Header.hpp"

class ServerHeader : public Header
{
	public:
		ServerHeader();
		ServerHeader(const ServerHeader& other);
		virtual ~ServerHeader();
		ServerHeader& operator=(const ServerHeader& other);

		std::string getType() const;
		Header* clone() const;
};

#endif
