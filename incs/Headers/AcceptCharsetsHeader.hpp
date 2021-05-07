#ifndef ACCEPT_CHARSETS_HEADER_HPP
#define ACCEPT_CHARSETS_HEADER_HPP

#include "PreferenceHeader.hpp"

class AcceptCharsetsHeader : public PreferenceHeader
{
	public:
		AcceptCharsetsHeader();
		AcceptCharsetsHeader(const AcceptCharsetsHeader& other);
		virtual ~AcceptCharsetsHeader();
		AcceptCharsetsHeader& operator=(const AcceptCharsetsHeader& other);

		std::string getType() const;
		Header* clone() const;
};

#endif
