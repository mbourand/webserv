#include "AcceptLanguageHeader.hpp"

AcceptLanguageHeader::AcceptLanguageHeader() {}
AcceptLanguageHeader::AcceptLanguageHeader(const AcceptLanguageHeader& other) {}
AcceptLanguageHeader::~AcceptLanguageHeader() {}
AcceptLanguageHeader& AcceptLanguageHeader::operator=(const AcceptLanguageHeader& other) { return *this; }

std::string AcceptLanguageHeader::getType() { return "Accept-Language"; }

void AcceptLanguageHeader::parse(std::string content)
{

}
