#include "AllowHeader.hpp"

AllowHeader::AllowHeader() {}
AllowHeader::AllowHeader(const AllowHeader&) {}
AllowHeader::~AllowHeader() {}
AllowHeader& AllowHeader::operator=(const AllowHeader&) { return *this; }

std::string AllowHeader::getType() const { return "Allow"; }

Header* AllowHeader::clone() const { return new AllowHeader(); }
