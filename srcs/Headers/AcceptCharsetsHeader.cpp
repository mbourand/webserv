#include "AcceptCharsetsHeader.hpp"

AcceptCharsetsHeader::AcceptCharsetsHeader() {}
AcceptCharsetsHeader::AcceptCharsetsHeader(const AcceptCharsetsHeader& other) {}
AcceptCharsetsHeader::~AcceptCharsetsHeader() {}
AcceptCharsetsHeader& AcceptCharsetsHeader::operator=(const AcceptCharsetsHeader& other) { return *this; }

std::string AcceptCharsetsHeader::getType() { return "Accept-Charsets"; }

void AcceptCharsetsHeader::parse(std::string content)
{

}
