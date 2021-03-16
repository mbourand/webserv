#include "ContentLengthHeader.hpp"

ContentLengthHeader::ContentLengthHeader() {}
ContentLengthHeader::ContentLengthHeader(const ContentLengthHeader&) {}
ContentLengthHeader::~ContentLengthHeader() {}
ContentLengthHeader& ContentLengthHeader::operator=(const ContentLengthHeader&) { return *this; }

std::string ContentLengthHeader::getType() const { return "Content-Length"; }
Header* ContentLengthHeader::clone() const { return new ContentLengthHeader(); }
