#include "HostHeader.hpp"
#include <iostream>

HostHeader::HostHeader() {}
HostHeader::HostHeader(const HostHeader&) {}
HostHeader::~HostHeader() {}
HostHeader& HostHeader::operator=(const HostHeader&) { return *this; }

std::string HostHeader::getType() const { return "Host"; }
Header* HostHeader::clone() const { return new HostHeader(); }
