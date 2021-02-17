#include "AcceptCharsetsHeader.hpp"

AcceptCharsetsHeader::AcceptCharsetsHeader() {}
AcceptCharsetsHeader::AcceptCharsetsHeader(const AcceptCharsetsHeader& other) {}
AcceptCharsetsHeader::~AcceptCharsetsHeader() {}
AcceptCharsetsHeader& AcceptCharsetsHeader::operator=(const AcceptCharsetsHeader& other) { return *this; }

std::string AcceptCharsetsHeader::getType() const { return "Accept-Charsets"; }

Header* AcceptCharsetsHeader::clone() const { return new AcceptCharsetsHeader(); }
