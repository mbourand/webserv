#include "AllowHeader.hpp"

AllowHeader::AllowHeader() {}
AllowHeader::AllowHeader(const AllowHeader& other) {}
AllowHeader::~AllowHeader() {}
AllowHeader& AllowHeader::operator=(const AllowHeader& other) { return *this; }

std::string AllowHeader::getType() const { return "Allow"; }

Header* AllowHeader::clone() const { return new AllowHeader(); }
