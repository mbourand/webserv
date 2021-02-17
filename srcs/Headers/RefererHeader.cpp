#include "RefererHeader.hpp"

RefererHeader::RefererHeader() {}
RefererHeader::RefererHeader(const RefererHeader& other) {}
RefererHeader::~RefererHeader() {}
RefererHeader& RefererHeader::operator=(const RefererHeader& other) { return *this; }

std::string RefererHeader::getType() const { return "Referer"; }
Header* RefererHeader::clone() const { return new RefererHeader(); }
