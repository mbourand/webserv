#ifndef ACCEPT_LANGUAGE_HEADER_HPP
#define ACCEPT_LANGUAGE_HEADER_HPP

#include "IHeader.hpp"

class AcceptLanguageHeader : public IHeader
{
	public:
		AcceptLanguageHeader();
		AcceptLanguageHeader(const AcceptLanguageHeader& other);
		virtual ~AcceptLanguageHeader();
		AcceptLanguageHeader& operator=(const AcceptLanguageHeader& other);

		std::string getType();
		void parse(std::string content);
};

#endif
