#ifndef RETRY_AFTER_HEADER_HPP
#define RETRY_AFTER_HEADER_HPP

#include "IHeader.hpp"

class RetryAfterHeader : public IHeader
{
	public:
		RetryAfterHeader();
		RetryAfterHeader(const RetryAfterHeader& other);
		virtual ~RetryAfterHeader();
		RetryAfterHeader& operator=(const RetryAfterHeader& other);

		std::string getType();
		void parse(std::string content);
};

#endif
