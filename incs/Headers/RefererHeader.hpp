#ifndef REFERER_HEADER_HPP
#define REFERER_HEADER_HPP

#include "IHeader.hpp"

class RefererHeader : public IHeader
{
	public:
		RefererHeader();
		RefererHeader(const RefererHeader& other);
		virtual ~RefererHeader();
		RefererHeader& operator=(const RefererHeader& other);

		std::string getType();
		void parse(std::string content);
};

#endif
