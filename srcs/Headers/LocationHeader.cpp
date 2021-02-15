#include "LocationHeader.hpp"

LocationHeader::LocationHeader() {}
LocationHeader::LocationHeader(const LocationHeader& other) {}
LocationHeader::~LocationHeader() {}
LocationHeader& LocationHeader::operator=(const LocationHeader& other) { return *this; }

std::string LocationHeader::getType() { return "Location"; }

void LocationHeader::parse(std::string content)
{

}
