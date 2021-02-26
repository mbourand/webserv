#include "TransferEncodingHeader.hpp"

TransferEncodingHeader::TransferEncodingHeader() {}
TransferEncodingHeader::TransferEncodingHeader(const TransferEncodingHeader& other) {}
TransferEncodingHeader::~TransferEncodingHeader() {}
TransferEncodingHeader& TransferEncodingHeader::operator=(const TransferEncodingHeader& other) { return *this; }

std::string TransferEncodingHeader::getType() const { return "Transfer-Encoding"; }
Header* TransferEncodingHeader::clone() const { return new TransferEncodingHeader(); }
