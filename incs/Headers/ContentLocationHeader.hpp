#ifndef CONTENT_LOCATION_HEADER_HPP
#define CONTENT_LOCATION_HEADER_HPP

#include "IHeader.hpp"

class ContentLocationHeader : public IHeader
{
	public:
		ContentLocationHeader();
		ContentLocationHeader(const ContentLocationHeader& other);
		virtual ~ContentLocationHeader();
		ContentLocationHeader& operator=(const ContentLocationHeader& other);

		std::string getType();
		void parse(std::string content);
};

#endif
