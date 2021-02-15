#ifndef ALLOW_HEADER_HPP
#define ALLOW_HEADER_HPP

#include "IHeader.hpp"

class AllowHeader : public IHeader
{
	public:
		AllowHeader();
		AllowHeader(const AllowHeader& other);
		virtual ~AllowHeader();
		AllowHeader& operator=(const AllowHeader& other);

		std::string getType();
		void parse(std::string content);
};

#endif
