#include "ContentLengthHeader.hpp"

ContentLengthHeader::ContentLengthHeader() {}
ContentLengthHeader::ContentLengthHeader(const ContentLengthHeader& other) {}
ContentLengthHeader::~ContentLengthHeader() {}
ContentLengthHeader& ContentLengthHeader::operator=(const ContentLengthHeader& other) { return *this; }

std::string ContentLengthHeader::getType() { return "Content-Length"; }

void ContentLengthHeader::parse(std::string content)
{

}
