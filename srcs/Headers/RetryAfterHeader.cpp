#include "RetryAfterHeader.hpp"

RetryAfterHeader::RetryAfterHeader() {}
RetryAfterHeader::RetryAfterHeader(const RetryAfterHeader& other) {}
RetryAfterHeader::~RetryAfterHeader() {}
RetryAfterHeader& RetryAfterHeader::operator=(const RetryAfterHeader& other) { return *this; }

std::string RetryAfterHeader::getType() { return "Retry-After"; }

void RetryAfterHeader::parse(std::string content)
{

}
