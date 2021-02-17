#include "RetryAfterHeader.hpp"

RetryAfterHeader::RetryAfterHeader() {}
RetryAfterHeader::RetryAfterHeader(const RetryAfterHeader& other) {}
RetryAfterHeader::~RetryAfterHeader() {}
RetryAfterHeader& RetryAfterHeader::operator=(const RetryAfterHeader& other) { return *this; }

std::string RetryAfterHeader::getType() const { return "Retry-After"; }
Header* RetryAfterHeader::clone() const { return new RetryAfterHeader(); }
