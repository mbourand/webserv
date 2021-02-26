#include "DateHeader.hpp"

DateHeader::DateHeader() {}
DateHeader::DateHeader(const DateHeader& other) {}
DateHeader::~DateHeader() {}
DateHeader& DateHeader::operator=(const DateHeader& other) { return *this; }

std::string DateHeader::getType() const { return "Date"; }
Header* DateHeader::clone() const { return new DateHeader(); }
