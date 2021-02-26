#ifndef CONTENT_LENGTH_HEADER_HPP
#define CONTENT_LENGTH_HEADER_HPP

#include "Header.hpp"

class ContentLengthHeader : public Header
{
	public:
		ContentLengthHeader();
		ContentLengthHeader(const ContentLengthHeader& other);
		virtual ~ContentLengthHeader();
		ContentLengthHeader& operator=(const ContentLengthHeader& other);

		std::string getType() const;
		Header* clone() const;
};

#endif
