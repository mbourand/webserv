#ifndef CONTENT_LENGTH_HEADER_HPP
#define CONTENT_LENGTH_HEADER_HPP

#include "IHeader.hpp"

class ContentLengthHeader : public IHeader
{
	public:
		ContentLengthHeader();
		ContentLengthHeader(const ContentLengthHeader& other);
		virtual ~ContentLengthHeader();
		ContentLengthHeader& operator=(const ContentLengthHeader& other);

		std::string getType();
		void parse(std::string content);
};

#endif
