#include "AcceptEncodingHeader.hpp"

AcceptEncodingHeader::AcceptEncodingHeader() {}
AcceptEncodingHeader::AcceptEncodingHeader(const AcceptEncodingHeader&) {}
AcceptEncodingHeader::~AcceptEncodingHeader() {}
AcceptEncodingHeader& AcceptEncodingHeader::operator=(const AcceptEncodingHeader&) { return *this; }

std::string AcceptEncodingHeader::getType() const { return "Accept-Encoding"; }

Header* AcceptEncodingHeader::clone() const { return new AcceptEncodingHeader(); }
