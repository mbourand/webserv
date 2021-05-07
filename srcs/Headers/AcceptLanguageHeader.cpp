#include "AcceptLanguageHeader.hpp"
#include <map>
#include <vector>
#include <functional>
#include "Utils.hpp"
#include <iostream>

AcceptLanguageHeader::AcceptLanguageHeader() {}
AcceptLanguageHeader::AcceptLanguageHeader(const AcceptLanguageHeader&) {}
AcceptLanguageHeader::~AcceptLanguageHeader() {}
AcceptLanguageHeader& AcceptLanguageHeader::operator=(const AcceptLanguageHeader&) { return *this; }

std::string AcceptLanguageHeader::getType() const { return "Accept-Language"; }

Header* AcceptLanguageHeader::clone() const { return new AcceptLanguageHeader(); }
