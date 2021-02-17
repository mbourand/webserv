#ifndef LAST_MODIFIED_HEADER_HPP
#define LAST_MODIFIED_HEADER_HPP

#include "Header.hpp"

class LastModifiedHeader : public Header
{
	public:
		LastModifiedHeader();
		LastModifiedHeader(const LastModifiedHeader& other);
		virtual ~LastModifiedHeader();
		LastModifiedHeader& operator=(const LastModifiedHeader& other);

		std::string getType() const;
		Header* clone() const;
};

#endif
