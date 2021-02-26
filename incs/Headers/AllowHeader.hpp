#ifndef ALLOW_HEADER_HPP
#define ALLOW_HEADER_HPP

#include "Header.hpp"

class AllowHeader : public Header
{
	public:
		AllowHeader();
		AllowHeader(const AllowHeader& other);
		virtual ~AllowHeader();
		AllowHeader& operator=(const AllowHeader& other);

		std::string getType() const;
		Header* clone() const;
};

#endif
