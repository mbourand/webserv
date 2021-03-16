#include "LocationHeader.hpp"

LocationHeader::LocationHeader() {}
LocationHeader::LocationHeader(const LocationHeader&) {}
LocationHeader::~LocationHeader() {}
LocationHeader& LocationHeader::operator=(const LocationHeader&) { return *this; }

std::string LocationHeader::getType() const { return "Location"; }
Header* LocationHeader::clone() const { return new LocationHeader(); }
