#include "ContentLengthHeader.hpp"

ContentLengthHeader::ContentLengthHeader() {}
ContentLengthHeader::ContentLengthHeader(const ContentLengthHeader& other) {}
ContentLengthHeader::~ContentLengthHeader() {}
ContentLengthHeader& ContentLengthHeader::operator=(const ContentLengthHeader& other) { return *this; }

std::string ContentLengthHeader::getType() const { return "Content-Length"; }
Header* ContentLengthHeader::clone() const { return new ContentLengthHeader(); }
