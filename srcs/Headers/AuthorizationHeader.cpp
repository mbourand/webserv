#include "AuthorizationHeader.hpp"

AuthorizationHeader::AuthorizationHeader() {}
AuthorizationHeader::AuthorizationHeader(const AuthorizationHeader& other) {}
AuthorizationHeader::~AuthorizationHeader() {}
AuthorizationHeader& AuthorizationHeader::operator=(const AuthorizationHeader& other) { return *this; }

std::string AuthorizationHeader::getType() { return "Authorization"; }

void AuthorizationHeader::parse(std::string content)
{

}
