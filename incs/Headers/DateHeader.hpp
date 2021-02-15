#ifndef DATE_HEADER_HPP
#define DATE_HEADER_HPP

#include "IHeader.hpp"

class DateHeader : public IHeader
{
	public:
		DateHeader();
		DateHeader(const DateHeader& other);
		virtual ~DateHeader();
		DateHeader& operator=(const DateHeader& other);

		std::string getType();
		void parse(std::string content);
};

#endif
