#include "LastModifiedHeader.hpp"

LastModifiedHeader::LastModifiedHeader() {}
LastModifiedHeader::LastModifiedHeader(const LastModifiedHeader& other) {}
LastModifiedHeader::~LastModifiedHeader() {}
LastModifiedHeader& LastModifiedHeader::operator=(const LastModifiedHeader& other) { return *this; }

std::string LastModifiedHeader::getType() const { return "Last-Modified"; }
Header* LastModifiedHeader::clone() const { return new LastModifiedHeader(); }
