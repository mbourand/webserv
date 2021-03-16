#include "AuthorizationHeader.hpp"

AuthorizationHeader::AuthorizationHeader() {}
AuthorizationHeader::AuthorizationHeader(const AuthorizationHeader&) {}
AuthorizationHeader::~AuthorizationHeader() {}
AuthorizationHeader& AuthorizationHeader::operator=(const AuthorizationHeader&) { return *this; }

std::string AuthorizationHeader::getType() const { return "Authorization"; }
Header* AuthorizationHeader::clone() const { return new AuthorizationHeader(); }
