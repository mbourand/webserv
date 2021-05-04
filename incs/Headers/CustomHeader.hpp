#ifndef CUSTOM_HEADER_HPP
#define CUSTOM_HEADER_HPP

#include "Header.hpp"

class CustomHeader : public Header
{
	public:
		CustomHeader(const std::string &type);
		CustomHeader(const CustomHeader& other);
		virtual ~CustomHeader();
		CustomHeader& operator=(const CustomHeader& other);

		std::string getType() const;
		Header* clone() const;

	private:

		std::string	_type;
};

#endif
