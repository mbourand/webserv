#include "WWWAuthenticateHeader.hpp"

WWWAuthenticateHeader::WWWAuthenticateHeader() {}
WWWAuthenticateHeader::WWWAuthenticateHeader(const WWWAuthenticateHeader& other) {}
WWWAuthenticateHeader::~WWWAuthenticateHeader() {}
WWWAuthenticateHeader& WWWAuthenticateHeader::operator=(const WWWAuthenticateHeader& other) { return *this; }

std::string WWWAuthenticateHeader::getType() const { return "WWW-Authenticate"; }
Header* WWWAuthenticateHeader::clone() const { return new WWWAuthenticateHeader(); }
