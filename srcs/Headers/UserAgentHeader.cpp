#include "UserAgentHeader.hpp"

UserAgentHeader::UserAgentHeader() {}
UserAgentHeader::UserAgentHeader(const UserAgentHeader& other) {}
UserAgentHeader::~UserAgentHeader() {}
UserAgentHeader& UserAgentHeader::operator=(const UserAgentHeader& other) { return *this; }

std::string UserAgentHeader::getType() const { return "User-Agent"; }
Header* UserAgentHeader::clone() const { return new UserAgentHeader(); }
