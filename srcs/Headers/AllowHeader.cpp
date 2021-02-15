#include "AllowHeader.hpp"

AllowHeader::AllowHeader() {}
AllowHeader::AllowHeader(const AllowHeader& other) {}
AllowHeader::~AllowHeader() {}
AllowHeader& AllowHeader::operator=(const AllowHeader& other) { return *this; }

std::string AllowHeader::getType() { return "Allow"; }

void AllowHeader::parse(std::string content)
{

}
