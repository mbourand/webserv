#include "WWWAuthenticateHeader.hpp"

WWWAuthenticateHeader::WWWAuthenticateHeader() {}
WWWAuthenticateHeader::WWWAuthenticateHeader(const WWWAuthenticateHeader& other) {}
WWWAuthenticateHeader::~WWWAuthenticateHeader() {}
WWWAuthenticateHeader& WWWAuthenticateHeader::operator=(const WWWAuthenticateHeader& other) { return *this; }

std::string WWWAuthenticateHeader::getType() { return "WWW-Authenticate"; }

void WWWAuthenticateHeader::parse(std::string content)
{

}
