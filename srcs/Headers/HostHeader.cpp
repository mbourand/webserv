#include "HostHeader.hpp"

HostHeader::HostHeader() {}
HostHeader::HostHeader(const HostHeader& other) {}
HostHeader::~HostHeader() {}
HostHeader& HostHeader::operator=(const HostHeader& other) { return *this; }

std::string HostHeader::getType() { return "Host"; }

void HostHeader::parse(std::string content)
{

}
