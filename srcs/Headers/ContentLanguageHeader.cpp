#include "ContentLanguageHeader.hpp"

ContentLanguageHeader::ContentLanguageHeader() {}
ContentLanguageHeader::ContentLanguageHeader(const ContentLanguageHeader& other) {}
ContentLanguageHeader::~ContentLanguageHeader() {}
ContentLanguageHeader& ContentLanguageHeader::operator=(const ContentLanguageHeader& other) { return *this; }

std::string ContentLanguageHeader::getType() { return "Content-Language"; }

void ContentLanguageHeader::parse(std::string content)
{

}
