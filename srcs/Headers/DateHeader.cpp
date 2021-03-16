#include "DateHeader.hpp"

DateHeader::DateHeader() {}
DateHeader::DateHeader(const DateHeader&) {}
DateHeader::~DateHeader() {}
DateHeader& DateHeader::operator=(const DateHeader&) { return *this; }

std::string DateHeader::getType() const { return "Date"; }
Header* DateHeader::clone() const { return new DateHeader(); }
