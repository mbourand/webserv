#include "LastModifiedHeader.hpp"

LastModifiedHeader::LastModifiedHeader() {}
LastModifiedHeader::LastModifiedHeader(const LastModifiedHeader& other) {}
LastModifiedHeader::~LastModifiedHeader() {}
LastModifiedHeader& LastModifiedHeader::operator=(const LastModifiedHeader& other) { return *this; }

std::string LastModifiedHeader::getType() { return "Last-Modified"; }

void LastModifiedHeader::parse(std::string content)
{

}
