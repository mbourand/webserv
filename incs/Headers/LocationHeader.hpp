#ifndef LOCATION_HEADER_HPP
#define LOCATION_HEADER_HPP

#include "Header.hpp"

class LocationHeader : public Header
{
	public:
		LocationHeader();
		LocationHeader(const LocationHeader& other);
		virtual ~LocationHeader();
		LocationHeader& operator=(const LocationHeader& other);

		std::string getType() const;
		Header* clone() const;
};

#endif
