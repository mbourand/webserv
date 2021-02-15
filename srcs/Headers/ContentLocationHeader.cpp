#include "ContentLocationHeader.hpp"

ContentLocationHeader::ContentLocationHeader() {}
ContentLocationHeader::ContentLocationHeader(const ContentLocationHeader& other) {}
ContentLocationHeader::~ContentLocationHeader() {}
ContentLocationHeader& ContentLocationHeader::operator=(const ContentLocationHeader& other) { return *this; }

std::string ContentLocationHeader::getType() { return "Content-Location"; }

void ContentLocationHeader::parse(std::string content)
{

}
