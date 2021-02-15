#include "TransferEncodingHeader.hpp"

TransferEncodingHeader::TransferEncodingHeader() {}
TransferEncodingHeader::TransferEncodingHeader(const TransferEncodingHeader& other) {}
TransferEncodingHeader::~TransferEncodingHeader() {}
TransferEncodingHeader& TransferEncodingHeader::operator=(const TransferEncodingHeader& other) { return *this; }

std::string TransferEncodingHeader::getType() { return "Transfer-Encoding"; }

void TransferEncodingHeader::parse(std::string content)
{

}
