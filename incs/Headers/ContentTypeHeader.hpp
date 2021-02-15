#ifndef CONTENT_TYPE_HEADER_HPP
#define CONTENT_TYPE_HEADER_HPP

#include "IHeader.hpp"

class ContentTypeHeader : public IHeader
{
	public:
		ContentTypeHeader();
		ContentTypeHeader(const ContentTypeHeader& other);
		virtual ~ContentTypeHeader();
		ContentTypeHeader& operator=(const ContentTypeHeader& other);

		std::string getType();
		void parse(std::string content);
};

#endif
