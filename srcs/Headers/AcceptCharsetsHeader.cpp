#include "AcceptCharsetsHeader.hpp"

AcceptCharsetsHeader::AcceptCharsetsHeader() {}
AcceptCharsetsHeader::AcceptCharsetsHeader(const AcceptCharsetsHeader&) {}
AcceptCharsetsHeader::~AcceptCharsetsHeader() {}
AcceptCharsetsHeader& AcceptCharsetsHeader::operator=(const AcceptCharsetsHeader&) { return *this; }

std::string AcceptCharsetsHeader::getType() const { return "Accept-Charsets"; }

Header* AcceptCharsetsHeader::clone() const { return new AcceptCharsetsHeader(); }
