#ifndef CONTENT_LANGUAGE_HEADER_HPP
#define CONTENT_LANGUAGE_HEADER_HPP

#include "Header.hpp"

class ContentLanguageHeader : public Header
{
	public:
		ContentLanguageHeader();
		ContentLanguageHeader(const ContentLanguageHeader& other);
		virtual ~ContentLanguageHeader();
		ContentLanguageHeader& operator=(const ContentLanguageHeader& other);

		std::string getType() const;
		Header* clone() const;
};

#endif
