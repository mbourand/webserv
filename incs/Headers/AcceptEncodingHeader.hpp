#ifndef ACCEPT_ENCODING_HEADER_HPP
#define ACCEPT_ENCODING_HEADER_HPP

#include "PreferenceHeader.hpp"

class AcceptEncodingHeader : public PreferenceHeader
{
	public:
		AcceptEncodingHeader();
		AcceptEncodingHeader(const AcceptEncodingHeader& other);
		virtual ~AcceptEncodingHeader();
		AcceptEncodingHeader& operator=(const AcceptEncodingHeader& other);

		std::string getType() const;
		Header* clone() const;
};

#endif
