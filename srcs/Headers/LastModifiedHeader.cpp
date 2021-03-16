#include "LastModifiedHeader.hpp"

LastModifiedHeader::LastModifiedHeader() {}
LastModifiedHeader::LastModifiedHeader(const LastModifiedHeader&) {}
LastModifiedHeader::~LastModifiedHeader() {}
LastModifiedHeader& LastModifiedHeader::operator=(const LastModifiedHeader&) { return *this; }

std::string LastModifiedHeader::getType() const { return "Last-Modified"; }
Header* LastModifiedHeader::clone() const { return new LastModifiedHeader(); }
