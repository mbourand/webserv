#ifndef CONTENT_TYPE_HEADER_HPP
#define CONTENT_TYPE_HEADER_HPP

#include "Header.hpp"

class ContentTypeHeader : public Header
{
	public:
		ContentTypeHeader();
		ContentTypeHeader(const ContentTypeHeader& other);
		virtual ~ContentTypeHeader();
		ContentTypeHeader& operator=(const ContentTypeHeader& other);

		std::string getType() const;
		Header* clone() const;
};

#endif
