#ifndef HOST_HEADER_HPP
#define HOST_HEADER_HPP

#include "Header.hpp"

class HostHeader : public Header
{
	public:
		HostHeader();
		HostHeader(const HostHeader& other);
		virtual ~HostHeader();
		HostHeader& operator=(const HostHeader& other);

		std::string getType() const;
		Header* clone() const;
};

#endif
