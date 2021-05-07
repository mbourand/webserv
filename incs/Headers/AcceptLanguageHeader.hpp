#ifndef ACCEPT_LANGUAGE_HEADER_HPP
#define ACCEPT_LANGUAGE_HEADER_HPP

#include "PreferenceHeader.hpp"
#include <map>

class AcceptLanguageHeader : public PreferenceHeader
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
