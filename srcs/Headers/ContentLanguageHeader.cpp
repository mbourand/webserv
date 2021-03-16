#include "ContentLanguageHeader.hpp"

ContentLanguageHeader::ContentLanguageHeader() {}
ContentLanguageHeader::ContentLanguageHeader(const ContentLanguageHeader&) {}
ContentLanguageHeader::~ContentLanguageHeader() {}
ContentLanguageHeader& ContentLanguageHeader::operator=(const ContentLanguageHeader&) { return *this; }

std::string ContentLanguageHeader::getType() const { return "Content-Language"; }
Header* ContentLanguageHeader::clone() const { return new ContentLanguageHeader(); }
