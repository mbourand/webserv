#ifndef ACCEPT_LANGUAGE_HEADER_HPP
#define ACCEPT_LANGUAGE_HEADER_HPP

#include "Header.hpp"

class AcceptLanguageHeader : public Header
{
	public:
		AcceptLanguageHeader();
		AcceptLanguageHeader(const AcceptLanguageHeader& other);
		virtual ~AcceptLanguageHeader();
		AcceptLanguageHeader& operator=(const AcceptLanguageHeader& other);

		std::string getType() const;
		Header* clone() const;
};

#endif
