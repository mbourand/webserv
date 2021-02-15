#ifndef SERVER_HEADER_HPP
#define SERVER_HEADER_HPP

#include "IHeader.hpp"

class ServerHeader : public IHeader
{
	public:
		ServerHeader();
		ServerHeader(const ServerHeader& other);
		virtual ~ServerHeader();
		ServerHeader& operator=(const ServerHeader& other);

		std::string getType();
		void parse(std::string content);
};

#endif
