#ifndef TRANSFER_ENCODING_HEADER_HPP
#define TRANSFER_ENCODING_HEADER_HPP

#include "IHeader.hpp"

class TransferEncodingHeader : public IHeader
{
	public:
		TransferEncodingHeader();
		TransferEncodingHeader(const TransferEncodingHeader& other);
		virtual ~TransferEncodingHeader();
		TransferEncodingHeader& operator=(const TransferEncodingHeader& other);

		std::string getType();
		void parse(std::string content);
};

#endif
