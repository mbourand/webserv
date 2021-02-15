#ifndef HOST_HEADER_HPP
#define HOST_HEADER_HPP

#include "IHeader.hpp"

class HostHeader : public IHeader
{
	public:
		HostHeader();
		HostHeader(const HostHeader& other);
		virtual ~HostHeader();
		HostHeader& operator=(const HostHeader& other);

		std::string getType();
		void parse(std::string content);
};

#endif
