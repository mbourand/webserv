#ifndef IHEADER_HPP
#define IHEADER_HPP

#include <string>

class IHeader
{
	public:
		virtual ~IHeader() {};

		std::string getType() const;
		IHeader* clone() const;
		void parse(std::string value);
};

#endif
