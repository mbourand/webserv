#include "ServerHeader.hpp"

ServerHeader::ServerHeader() {}
ServerHeader::ServerHeader(const ServerHeader&) {}
ServerHeader::~ServerHeader() {}
ServerHeader& ServerHeader::operator=(const ServerHeader&) { return *this; }

std::string ServerHeader::getType() const { return "Server"; }
Header* ServerHeader::clone() const { return new ServerHeader(); }
