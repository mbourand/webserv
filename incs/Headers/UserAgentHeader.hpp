#ifndef USER_AGENT_HEADER_HPP
#define USER_AGENT_HEADER_HPP

#include "IHeader.hpp"

class UserAgentHeader : public IHeader
{
	public:
		UserAgentHeader();
		UserAgentHeader(const UserAgentHeader& other);
		virtual ~UserAgentHeader();
		UserAgentHeader& operator=(const UserAgentHeader& other);

		std::string getType();
		void parse(std::string content);
};

#endif
