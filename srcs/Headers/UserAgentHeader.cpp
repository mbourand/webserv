#include "UserAgentHeader.hpp"

UserAgentHeader::UserAgentHeader() {}
UserAgentHeader::UserAgentHeader(const UserAgentHeader& other) {}
UserAgentHeader::~UserAgentHeader() {}
UserAgentHeader& UserAgentHeader::operator=(const UserAgentHeader& other) { return *this; }

std::string UserAgentHeader::getType() { return "User-Agent"; }

void UserAgentHeader::parse(std::string content)
{

}
