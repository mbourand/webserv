#include "ContentLocationHeader.hpp"

ContentLocationHeader::ContentLocationHeader() {}
ContentLocationHeader::ContentLocationHeader(const ContentLocationHeader&) {}
ContentLocationHeader::~ContentLocationHeader() {}
ContentLocationHeader& ContentLocationHeader::operator=(const ContentLocationHeader&) { return *this; }

std::string ContentLocationHeader::getType() const { return "Content-Location"; }
Header* ContentLocationHeader::clone() const { return new ContentLocationHeader(); }
