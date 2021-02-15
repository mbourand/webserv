#include "ContentTypeHeader.hpp"

ContentTypeHeader::ContentTypeHeader() {}
ContentTypeHeader::ContentTypeHeader(const ContentTypeHeader& other) {}
ContentTypeHeader::~ContentTypeHeader() {}
ContentTypeHeader& ContentTypeHeader::operator=(const ContentTypeHeader& other) { return *this; }

std::string ContentTypeHeader::getType() { return "Content-Type"; }

void ContentTypeHeader::parse(std::string content)
{

}
