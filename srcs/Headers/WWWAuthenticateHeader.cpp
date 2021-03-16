#include "WWWAuthenticateHeader.hpp"

WWWAuthenticateHeader::WWWAuthenticateHeader() {}
WWWAuthenticateHeader::WWWAuthenticateHeader(const WWWAuthenticateHeader&) {}
WWWAuthenticateHeader::~WWWAuthenticateHeader() {}
WWWAuthenticateHeader& WWWAuthenticateHeader::operator=(const WWWAuthenticateHeader&) { return *this; }

std::string WWWAuthenticateHeader::getType() const { return "WWW-Authenticate"; }
Header* WWWAuthenticateHeader::clone() const { return new WWWAuthenticateHeader(); }
