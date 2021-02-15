#ifndef LAST_MODIFIED_HEADER_HPP
#define LAST_MODIFIED_HEADER_HPP

#include "IHeader.hpp"

class LastModifiedHeader : public IHeader
{
	public:
		LastModifiedHeader();
		LastModifiedHeader(const LastModifiedHeader& other);
		virtual ~LastModifiedHeader();
		LastModifiedHeader& operator=(const LastModifiedHeader& other);

		std::string getType();
		void parse(std::string content);
};

#endif
