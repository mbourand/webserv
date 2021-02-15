#include "ServerHeader.hpp"

ServerHeader::ServerHeader() {}
ServerHeader::ServerHeader(const ServerHeader& other) {}
ServerHeader::~ServerHeader() {}
ServerHeader& ServerHeader::operator=(const ServerHeader& other) { return *this; }

std::string ServerHeader::getType() { return "Server"; }

void ServerHeader::parse(std::string content)
{

}
