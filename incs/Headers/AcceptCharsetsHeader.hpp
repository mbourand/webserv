#ifndef ACCEPT_CHARSETS_HEADER_HPP
#define ACCEPT_CHARSETS_HEADER_HPP

#include "IHeader.hpp"

class AcceptCharsetsHeader : public IHeader
{
	public:
		AcceptCharsetsHeader();
		AcceptCharsetsHeader(const AcceptCharsetsHeader& other);
		virtual ~AcceptCharsetsHeader();
		AcceptCharsetsHeader& operator=(const AcceptCharsetsHeader& other);

		std::string getType();
		void parse(std::string content);
};

#endif
