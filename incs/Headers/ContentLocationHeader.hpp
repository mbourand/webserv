#ifndef CONTENT_LOCATION_HEADER_HPP
#define CONTENT_LOCATION_HEADER_HPP

#include "Header.hpp"

class ContentLocationHeader : public Header
{
	public:
		ContentLocationHeader();
		ContentLocationHeader(const ContentLocationHeader& other);
		virtual ~ContentLocationHeader();
		ContentLocationHeader& operator=(const ContentLocationHeader& other);

		std::string getType() const;
		Header* clone() const;
};

#endif
