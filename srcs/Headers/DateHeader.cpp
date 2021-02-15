#include "DateHeader.hpp"

DateHeader::DateHeader() {}
DateHeader::DateHeader(const DateHeader& other) {}
DateHeader::~DateHeader() {}
DateHeader& DateHeader::operator=(const DateHeader& other) { return *this; }

std::string DateHeader::getType() { return "Date"; }

void DateHeader::parse(std::string content)
{

}
