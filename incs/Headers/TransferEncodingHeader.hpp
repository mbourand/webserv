#ifndef TRANSFER_ENCODING_HEADER_HPP
#define TRANSFER_ENCODING_HEADER_HPP

#include "Header.hpp"

class TransferEncodingHeader : public Header
{
	public:
		TransferEncodingHeader();
		TransferEncodingHeader(const TransferEncodingHeader& other);
		virtual ~TransferEncodingHeader();
		TransferEncodingHeader& operator=(const TransferEncodingHeader& other);

		std::string getType() const;
		Header* clone() const;
};

#endif
