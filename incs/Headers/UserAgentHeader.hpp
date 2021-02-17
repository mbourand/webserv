#ifndef USER_AGENT_HEADER_HPP
#define USER_AGENT_HEADER_HPP

#include "Header.hpp"

class UserAgentHeader : public Header
{
	public:
		UserAgentHeader();
		UserAgentHeader(const UserAgentHeader& other);
		virtual ~UserAgentHeader();
		UserAgentHeader& operator=(const UserAgentHeader& other);

		std::string getType() const;
		Header* clone() const;
};

#endif
