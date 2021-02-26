#include "HostHeader.hpp"
#include <iostream>

HostHeader::HostHeader() {}
HostHeader::HostHeader(const HostHeader& other) {}
HostHeader::~HostHeader() {}
HostHeader& HostHeader::operator=(const HostHeader& other) { return *this; }

std::string HostHeader::getType() const { return "Host"; }
Header* HostHeader::clone() const { return new HostHeader(); }
