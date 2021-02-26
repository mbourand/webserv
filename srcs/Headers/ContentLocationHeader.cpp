#include "ContentLocationHeader.hpp"

ContentLocationHeader::ContentLocationHeader() {}
ContentLocationHeader::ContentLocationHeader(const ContentLocationHeader& other) {}
ContentLocationHeader::~ContentLocationHeader() {}
ContentLocationHeader& ContentLocationHeader::operator=(const ContentLocationHeader& other) { return *this; }

std::string ContentLocationHeader::getType() const { return "Content-Location"; }
Header* ContentLocationHeader::clone() const { return new ContentLocationHeader(); }
