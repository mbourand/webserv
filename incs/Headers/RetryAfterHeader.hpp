#ifndef RETRY_AFTER_HEADER_HPP
#define RETRY_AFTER_HEADER_HPP

#include "Header.hpp"

class RetryAfterHeader : public Header
{
	public:
		RetryAfterHeader();
		RetryAfterHeader(const RetryAfterHeader& other);
		virtual ~RetryAfterHeader();
		RetryAfterHeader& operator=(const RetryAfterHeader& other);

		std::string getType() const;
		Header* clone() const;
};

#endif
