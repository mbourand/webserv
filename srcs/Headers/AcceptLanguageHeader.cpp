#include "AcceptLanguageHeader.hpp"

AcceptLanguageHeader::AcceptLanguageHeader() {}
AcceptLanguageHeader::AcceptLanguageHeader(const AcceptLanguageHeader& other) {}
AcceptLanguageHeader::~AcceptLanguageHeader() {}
AcceptLanguageHeader& AcceptLanguageHeader::operator=(const AcceptLanguageHeader& other) { return *this; }

std::string AcceptLanguageHeader::getType() const { return "Accept-Language"; }

Header* AcceptLanguageHeader::clone() const { return new AcceptLanguageHeader(); }
