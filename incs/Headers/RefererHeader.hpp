#ifndef REFERER_HEADER_HPP
#define REFERER_HEADER_HPP

#include "Header.hpp"

class RefererHeader : public Header
{
	public:
		RefererHeader();
		RefererHeader(const RefererHeader& other);
		virtual ~RefererHeader();
		RefererHeader& operator=(const RefererHeader& other);

		std::string getType() const;
		Header* clone() const;
};

#endif
