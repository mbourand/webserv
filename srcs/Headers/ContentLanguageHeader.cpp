#include "ContentLanguageHeader.hpp"

ContentLanguageHeader::ContentLanguageHeader() {}
ContentLanguageHeader::ContentLanguageHeader(const ContentLanguageHeader& other) {}
ContentLanguageHeader::~ContentLanguageHeader() {}
ContentLanguageHeader& ContentLanguageHeader::operator=(const ContentLanguageHeader& other) { return *this; }

std::string ContentLanguageHeader::getType() const { return "Content-Language"; }
Header* ContentLanguageHeader::clone() const { return new ContentLanguageHeader(); }
