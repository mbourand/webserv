#ifndef LOCATION_HEADER_HPP
#define LOCATION_HEADER_HPP

#include "IHeader.hpp"

class LocationHeader : public IHeader
{
	public:
		LocationHeader();
		LocationHeader(const LocationHeader& other);
		virtual ~LocationHeader();
		LocationHeader& operator=(const LocationHeader& other);

		std::string getType();
		void parse(std::string content);
};

#endif
