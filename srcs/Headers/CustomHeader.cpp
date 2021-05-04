#include "CustomHeader.hpp"

CustomHeader::CustomHeader(const std::string &type) : _type(type) {}
CustomHeader::CustomHeader(const CustomHeader &other) : _type(other._type) {}
CustomHeader::~CustomHeader() {}
CustomHeader& CustomHeader::operator=(const CustomHeader&) { return *this; }

std::string CustomHeader::getType() const { return _type; }

Header* CustomHeader::clone() const { return new CustomHeader(_type); }
