#ifndef DATE_HEADER_HPP
#define DATE_HEADER_HPP

#include "Header.hpp"

class DateHeader : public Header
{
	public:
		DateHeader();
		DateHeader(const DateHeader& other);
		virtual ~DateHeader();
		DateHeader& operator=(const DateHeader& other);

		std::string getType() const;
		Header* clone() const;
};

#endif
