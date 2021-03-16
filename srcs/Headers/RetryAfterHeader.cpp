#include "RetryAfterHeader.hpp"

RetryAfterHeader::RetryAfterHeader() {}
RetryAfterHeader::RetryAfterHeader(const RetryAfterHeader&) {}
RetryAfterHeader::~RetryAfterHeader() {}
RetryAfterHeader& RetryAfterHeader::operator=(const RetryAfterHeader&) { return *this; }

std::string RetryAfterHeader::getType() const { return "Retry-After"; }
Header* RetryAfterHeader::clone() const { return new RetryAfterHeader(); }
