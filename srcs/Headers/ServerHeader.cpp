#include "ServerHeader.hpp"

ServerHeader::ServerHeader() {}
ServerHeader::ServerHeader(const ServerHeader& other) {}
ServerHeader::~ServerHeader() {}
ServerHeader& ServerHeader::operator=(const ServerHeader& other) { return *this; }

std::string ServerHeader::getType() const { return "Server"; }
Header* ServerHeader::clone() const { return new ServerHeader(); }
