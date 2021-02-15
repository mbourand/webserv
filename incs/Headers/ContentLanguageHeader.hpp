#ifndef CONTENT_LANGUAGE_HEADER_HPP
#define CONTENT_LANGUAGE_HEADER_HPP

#include "IHeader.hpp"

class ContentLanguageHeader : public IHeader
{
	public:
		ContentLanguageHeader();
		ContentLanguageHeader(const ContentLanguageHeader& other);
		virtual ~ContentLanguageHeader();
		ContentLanguageHeader& operator=(const ContentLanguageHeader& other);

		std::string getType();
		void parse(std::string content);
};

#endif
