#include "RefererHeader.hpp"

RefererHeader::RefererHeader() {}
RefererHeader::RefererHeader(const RefererHeader&) {}
RefererHeader::~RefererHeader() {}
RefererHeader& RefererHeader::operator=(const RefererHeader&) { return *this; }

std::string RefererHeader::getType() const { return "Referer"; }
Header* RefererHeader::clone() const { return new RefererHeader(); }
